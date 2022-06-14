use winit::{
	event::{Event, WindowEvent},
	event_loop::{ControlFlow, EventLoop},
	window::{Window, WindowBuilder},
};

use crate::renderer::Renderer;

#[derive(Debug)]
pub struct App {
	event_loop: EventLoop<()>,
	window: Window,
	renderer: Renderer,
}

impl App {
	// TODO: Add WindowConfig struct
	pub async fn new(window_name: &str) -> Self {
		let event_loop = EventLoop::new();

		let window = WindowBuilder::new().build(&event_loop).unwrap();
		window.set_title(format!("{}: {:?}", window_name, window.id()).as_str());

		let renderer = Renderer::new(&window).await;

		Self {
			event_loop,
			window,
			renderer,
		}
	}

	pub fn run(mut self) {
		self.event_loop.run(move |event, _, control_flow| {
			*control_flow = ControlFlow::Wait;

			match event {
				Event::WindowEvent { ref event, .. } => {
					match event {
						WindowEvent::CursorMoved { position, .. } => {
							self.renderer.set_mouse_position(position.x, position.y);
						}
						WindowEvent::CloseRequested
						| WindowEvent::KeyboardInput {
							input:
								winit::event::KeyboardInput {
									state: winit::event::ElementState::Pressed,
									virtual_keycode: Some(winit::event::VirtualKeyCode::Escape),
									..
								},
							..
						} => *control_flow = ControlFlow::Exit,
						WindowEvent::Resized(physical_size) => {
							self
								.renderer
								.set_surface_size(physical_size.width, physical_size.height);
						}
						WindowEvent::ScaleFactorChanged { new_inner_size, .. } => {
							self
								.renderer
								.set_surface_size(new_inner_size.width, new_inner_size.width);
						}
						_ => (),
					};
				}
				Event::RedrawRequested(_window_id) => {
					self.renderer.update();

					match self.renderer.render() {
						Ok(_) => (),
						Err(wgpu::SurfaceError::Lost) => self.renderer.reconfigure_surface(),
						Err(wgpu::SurfaceError::OutOfMemory) => *control_flow = ControlFlow::Exit,
						Err(e) => eprintln!("{:?}", e),
					};
				}
				Event::MainEventsCleared => {
					self.window.request_redraw();
				}
				_ => (),
			}
		});
	}
}
