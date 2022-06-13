use crate::{util::default, vertex::Vertex};

#[derive(Debug)]
pub struct Renderer {
	device: wgpu::Device,
	queue: wgpu::Queue,
	surface: wgpu::Surface, // TODO: maybe support multiple surfaces
	surface_config: wgpu::SurfaceConfiguration,
	render_pipeline: wgpu::RenderPipeline, // TODO: break up render_pipeline even more later on to use multiple shaders
}

impl Renderer {
	// TODO: Use custom Error
	// TODO: Generalize with new<W: raw_window_handle::HasRawWindowHandle>(window: &W)
	pub async fn new(window: &winit::window::Window) -> Self {
		let instance = wgpu::Instance::new(wgpu::Backends::PRIMARY);

		let surface = unsafe { instance.create_surface(window) };

		let adapter = instance
			.request_adapter(&wgpu::RequestAdapterOptions {
				power_preference: wgpu::PowerPreference::default(),
				force_fallback_adapter: false,
				compatible_surface: Some(&surface),
			})
			.await
			.unwrap();

		let (device, queue) = adapter
			.request_device(
				&wgpu::DeviceDescriptor {
					label: Some("Device"),
					features: wgpu::Features::empty(),
					limits: wgpu::Limits::downlevel_defaults(),
				},
				None,
			)
			.await
			.unwrap();

		let surface_config = wgpu::SurfaceConfiguration {
			usage: wgpu::TextureUsages::RENDER_ATTACHMENT,
			format: wgpu::TextureFormat::Bgra8UnormSrgb,
			width: window.inner_size().width,
			height: window.inner_size().height,
			present_mode: wgpu::PresentMode::Fifo,
		};

		let render_pipeline = Self::new_render_pipline(&device, &surface_config);

		Self {
			device,
			queue,
			surface,
			surface_config,
			render_pipeline,
		}
	}

	fn new_render_pipline(
		device: &wgpu::Device,
		surface_config: &wgpu::SurfaceConfiguration,
	) -> wgpu::RenderPipeline {
		let pipeline_layout = device.create_pipeline_layout(&wgpu::PipelineLayoutDescriptor {
			label: Some("Render Pipeline Layout"),
			bind_group_layouts: &[],
			push_constant_ranges: &[],
		});

		let shader = device.create_shader_module(&wgpu::include_wgsl!("test.wgsl"));

		device.create_render_pipeline(&wgpu::RenderPipelineDescriptor {
			label: Some("Render Pipeline"),
			layout: Some(&pipeline_layout),
			vertex: wgpu::VertexState {
				module: &shader,
				entry_point: "vs_main",
				buffers: &[Vertex::layout()],
			},
			primitive: wgpu::PrimitiveState {
				topology: wgpu::PrimitiveTopology::TriangleList,
				front_face: wgpu::FrontFace::Ccw,
				cull_mode: Some(wgpu::Face::Back),
				strip_index_format: None,
				..default()
			},
			depth_stencil: None,
			multisample: wgpu::MultisampleState::default(),
			fragment: Some(wgpu::FragmentState {
				module: &shader,
				entry_point: "fs_sdf_box",
				targets: &[wgpu::ColorTargetState {
					format: surface_config.format,
					blend: Some(wgpu::BlendState::ALPHA_BLENDING),
					write_mask: wgpu::ColorWrites::ALL,
				}],
			}),
			multiview: None,
		})
	}

	pub fn set_mouse_position(&self, _x: f64, _y: f64) {}

	pub fn set_surface_size(&mut self, width: u32, height: u32) {
		self.surface_config.width = width;
		self.surface_config.height = height;
		self.surface.configure(&self.device, &self.surface_config);
	}

	pub fn reconfigure_surface(&self) {
		self.surface.configure(&self.device, &self.surface_config);
	}

	pub fn _update(&mut self) {}

	pub fn render(&mut self) -> Result<(), wgpu::SurfaceError> {
		let output = self.surface.get_current_texture()?;
		let view = output
			.texture
			.create_view(&wgpu::TextureViewDescriptor::default());

		let mut encoder = self
			.device
			.create_command_encoder(&wgpu::CommandEncoderDescriptor {
				label: Some("Command Encoder"),
			});

		{
			let mut render_pass = encoder.begin_render_pass(&wgpu::RenderPassDescriptor {
				label: Some("Render Pass"),
				color_attachments: &[wgpu::RenderPassColorAttachment {
					view: &view,
					resolve_target: None,
					ops: wgpu::Operations {
						load: wgpu::LoadOp::Clear(wgpu::Color {
							r: 1.0,
							g: 0.0,
							b: 1.0,
							a: 0.0,
						}),
						store: false,
					},
				}],
				depth_stencil_attachment: None,
			});

			render_pass.set_pipeline(&self.render_pipeline);
			// render_pass.set_vertex_buffer(0, buffer_slice);
			// render_pass.set_index_buffer(buffer_slice, wgpu::IndexFormat::Uint16);
			// render_pass.draw_indexed(0..6, 0, 0..1);
		}

		self.queue.submit(std::iter::once(encoder.finish()));
		output.present();

		Ok(())
	}
}
