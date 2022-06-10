use std::collections::HashMap;
use winit::{
	event::{Event, WindowEvent},
	event_loop::{ControlFlow, EventLoop},
	window::{Window, WindowBuilder, WindowId},
};

#[derive(Debug)]
pub struct App {
	event_loop: EventLoop<()>,
	windows: HashMap<WindowId, Window>,
	viewports: HashMap<WindowId, Viewport>,
}

impl App {
	pub fn new() -> Self {
		Self {
			event_loop: EventLoop::new(),
			windows: HashMap::new(),
			viewports: HashMap::new(),
		}
	}

	pub async fn spawn_window(&mut self, window_name: &str) {
		// TODO: Add WindowConfig struct
		if let Ok(window) = WindowBuilder::new().build(&self.event_loop) {
			window.set_title(format!("{}: {:?}", window_name, window.id()).as_str());
			self
				.viewports
				.insert(window.id(), Viewport::new(&window).await);
			self.windows.insert(window.id(), window);
		}
	}

	pub fn set_render_fn(f: impl Fn() -> Result<(), wgpu::SurfaceError>) {}

	pub fn run(mut self) {
		self.event_loop.run(move |event, _, control_flow| {
			*control_flow = ControlFlow::Wait;

			match event {
				Event::WindowEvent {
					ref event,
					ref window_id,
				} => {
					match event {
						WindowEvent::CloseRequested
						| WindowEvent::KeyboardInput {
							input:
								winit::event::KeyboardInput {
									state: winit::event::ElementState::Pressed,
									virtual_keycode: Some(winit::event::VirtualKeyCode::Escape),
									..
								},
							..
						} => {
							self.windows.remove(window_id);
							self.viewports.remove(window_id);

							if self.windows.is_empty() {
								*control_flow = ControlFlow::Exit
							};
						}
						WindowEvent::Resized(physical_size) => {
							if let Some(viewport) = self.viewports.get_mut(window_id) {
								viewport.resize(*physical_size);
							}
						}
						WindowEvent::ScaleFactorChanged { new_inner_size, .. } => {
							if let Some(viewport) = self.viewports.get_mut(window_id) {
								viewport.resize(**new_inner_size);
							}
						}
						_ => (),
					};
				}
				Event::RedrawRequested(window_id) => {
					if let Some(viewport) = self.viewports.get_mut(&window_id) {
						match viewport.render() {
							Ok(_) => (),
							Err(wgpu::SurfaceError::Lost) => viewport.resize(viewport.size),
							Err(wgpu::SurfaceError::OutOfMemory) => *control_flow = ControlFlow::Exit,
							Err(e) => eprintln!("{:?}", e),
						};
					}
				}
				Event::MainEventsCleared => {
					for (_, window) in &self.windows {
						window.request_redraw();
					}
				}
				_ => (),
			}
		});
	}
}

impl Default for App {
	fn default() -> Self {
		Self::new()
	}
}

struct Viewport {
	surface: wgpu::Surface,
	device: wgpu::Device,
	queue: wgpu::Queue,
	config: wgpu::SurfaceConfiguration,
	size: winit::dpi::PhysicalSize<u32>,
	render: Box<dyn Fn() -> Result<(), wgpu::SurfaceError>>,
}

impl Viewport {
	async fn new(window: &Window) -> Self {
		// The instance is a handle to the GPU
		// Backends::all ==> Vulkan | Metal | DX12 | Browser WebGPU
		let instance = wgpu::Instance::new(wgpu::Backends::all());
		let surface = unsafe { instance.create_surface(window) };

		let adapter = instance
			// TODO: Check all available adapters and pick the best / one that works
			// https://docs.rs/wgpu/latest/wgpu/struct.Adapter.html
			// https://sotrh.github.io/learn-wgpu/beginner/tutorial2-surface/#state-new
			.request_adapter(&wgpu::RequestAdapterOptionsBase {
				power_preference: wgpu::PowerPreference::default(),
				force_fallback_adapter: false,
				compatible_surface: Some(&surface),
			})
			.await
			.unwrap();

		let (device, queue) = adapter
			.request_device(
				&wgpu::DeviceDescriptor {
					features: wgpu::Features::empty(),
					limits: wgpu::Limits::default(),
					label: None,
				},
				None,
			)
			.await
			.unwrap();

		let size = window.inner_size();
		let config = wgpu::SurfaceConfiguration {
			usage: wgpu::TextureUsages::RENDER_ATTACHMENT,
			format: surface.get_preferred_format(&adapter).unwrap(),
			width: size.width,
			height: size.height,
			present_mode: wgpu::PresentMode::Fifo,
		};

		Self {
			surface,
			device,
			queue,
			config,
			size,
		}
	}

	fn resize(&mut self, new_size: winit::dpi::PhysicalSize<u32>) {
		if new_size.width > 0 && new_size.height > 0 {
			self.size = new_size;
			self.config.width = new_size.width;
			self.config.height = new_size.height;
			self.surface.configure(&self.device, &self.config);
		}
	}

	pub fn set_render_fn(f: impl Fn() -> Result<(), wgpu::SurfaceError>) {}

	fn render(&self) -> Result<(), wgpu::SurfaceError> {
		let output = self.surface.get_current_texture()?;
		let view = output
			.texture
			.create_view(&wgpu::TextureViewDescriptor::default());
		let mut encoder = self
			.device
			.create_command_encoder(&wgpu::CommandEncoderDescriptor {
				label: Some("Render Encoder"),
			});

		{
			let _render_pass = encoder.begin_render_pass(&wgpu::RenderPassDescriptor {
				label: Some("Render Pass"),
				color_attachments: &[wgpu::RenderPassColorAttachment {
					view: &view,
					resolve_target: None,
					ops: wgpu::Operations {
						load: wgpu::LoadOp::Clear(wgpu::Color {
							r: 0.1,
							g: 0.1,
							b: 0.1,
							a: 1.0,
						}),
						store: true,
					},
				}],
				depth_stencil_attachment: None,
			});
		}

		self.queue.submit(std::iter::once(encoder.finish()));
		output.present();

		Ok(())
	}
}
