use wgpu::{util::DeviceExt, BufferUsages, ShaderStages};
use winit::{dpi::PhysicalPosition, window::Window};

use crate::{color::Color, mesh::*, vertex::*};

#[derive(Debug)]
pub struct SurfaceState {
	surface: wgpu::Surface,
	device: wgpu::Device,
	queue: wgpu::Queue,
	config: wgpu::SurfaceConfiguration,
	pub(crate) size: winit::dpi::PhysicalSize<u32>,
	pub(crate) cursor_pos: PhysicalPosition<f64>,
	render_pipeline: wgpu::RenderPipeline,
	mesh_buffer: MeshBuffer,
	frag_bind_group: wgpu::BindGroup,
	colors_buffer: wgpu::Buffer,
	colors: Vec<Color>,
}

impl SurfaceState {
	pub async fn new(window: &Window) -> Self {
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

		let shader = device.create_shader_module(&wgpu::ShaderModuleDescriptor {
			label: Some("Solid Color"),
			source: wgpu::ShaderSource::Wgsl(include_str!("test.wgsl").into()),
		});

		let frag_bind_group_layout =
			device.create_bind_group_layout(&wgpu::BindGroupLayoutDescriptor {
				label: Some("Fragment Bind Group Layout"),
				entries: &[wgpu::BindGroupLayoutEntry {
					binding: 0,
					visibility: ShaderStages::FRAGMENT,
					ty: wgpu::BindingType::Buffer {
						ty: wgpu::BufferBindingType::Uniform,
						has_dynamic_offset: false,
						min_binding_size: None,
					},
					count: None,
				}],
			});

		let colors: Vec<Color> = vec![
			[1.0, 0.0, 0.0, 1.0],
			[0.0, 1.0, 0.0, 1.0],
			[0.0, 0.0, 1.0, 1.0],
		];

		let colors_buffer = device.create_buffer_init(&wgpu::util::BufferInitDescriptor {
			label: Some("Colors Buffer"),
			contents: bytemuck::cast_slice(colors.as_slice()),
			usage: wgpu::BufferUsages::COPY_DST | wgpu::BufferUsages::UNIFORM,
		});

		let frag_bind_group = device.create_bind_group(&wgpu::BindGroupDescriptor {
			label: Some("Fragment Bind Group"),
			layout: &frag_bind_group_layout,
			entries: &[wgpu::BindGroupEntry {
				binding: 0,
				resource: colors_buffer.as_entire_binding(),
			}],
		});

		let render_pipeline_layout = device.create_pipeline_layout(&wgpu::PipelineLayoutDescriptor {
			label: Some("Render Pipeline Layout"),
			bind_group_layouts: &[&frag_bind_group_layout],
			push_constant_ranges: &[],
		});

		let render_pipeline_desc = wgpu::RenderPipelineDescriptor {
			label: Some("Render Pipeline"),
			layout: Some(&render_pipeline_layout),
			vertex: wgpu::VertexState {
				module: &shader,
				entry_point: "vs_main",
				buffers: &[Vertex::layout()],
			},
			fragment: None,
			primitive: wgpu::PrimitiveState {
				topology: wgpu::PrimitiveTopology::TriangleList,
				strip_index_format: None,
				front_face: wgpu::FrontFace::Ccw,
				cull_mode: Some(wgpu::Face::Back),
				polygon_mode: wgpu::PolygonMode::Fill,
				unclipped_depth: false,
				conservative: false,
			},
			depth_stencil: None,
			multisample: wgpu::MultisampleState::default(),
			multiview: None,
		};

		let render_pipeline = device.create_render_pipeline(&wgpu::RenderPipelineDescriptor {
			fragment: Some(wgpu::FragmentState {
				module: &shader,
				entry_point: "fs_debug_uv",
				targets: &[wgpu::ColorTargetState {
					format: config.format,
					blend: Some(wgpu::BlendState::REPLACE),
					write_mask: wgpu::ColorWrites::ALL,
				}],
			}),
			..render_pipeline_desc
		});

		let quad1 = Mesh::new_quad(1.0, 1.0, 0);
		let quad2 = Mesh::new_quad(-1.0, -1.0, 2);
		let batch = Mesh::make_batched(vec![quad1, quad2]);

		let mesh_buffer = MeshBuffer {
			num_vertices: batch.indices.len() as u32,
			vertex_buffer: device.create_buffer_init(&wgpu::util::BufferInitDescriptor {
				label: Some("Batched Vertex Buffer"),
				usage: BufferUsages::VERTEX,
				contents: bytemuck::cast_slice(batch.vertices.as_slice()),
			}),
			index_buffer: device.create_buffer_init(&wgpu::util::BufferInitDescriptor {
				label: Some("Batched Index Buffer"),
				usage: BufferUsages::INDEX,
				contents: bytemuck::cast_slice(batch.indices.as_slice()),
			}),
		};

		Self {
			surface,
			device,
			queue,
			config,
			size,
			cursor_pos: PhysicalPosition::default(),
			render_pipeline,
			mesh_buffer,
			frag_bind_group,
			colors_buffer,
			colors,
		}
	}

	pub fn resize(&mut self, new_size: winit::dpi::PhysicalSize<u32>) {
		if new_size.width > 0 && new_size.height > 0 {
			self.size = new_size;
			self.config.width = new_size.width;
			self.config.height = new_size.height;
			self.surface.configure(&self.device, &self.config);
		}
	}

	pub fn render(&mut self) -> Result<(), wgpu::SurfaceError> {
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
							a: 1.0,
						}),
						store: true,
					},
				}],
				depth_stencil_attachment: None,
			});

			render_pass.set_pipeline(&self.render_pipeline);

			// render_pass.set_bind_group(0, bind_group, &[]);
			render_pass.set_bind_group(0, &self.frag_bind_group, &[]);

			render_pass.set_vertex_buffer(0, self.mesh_buffer.vertex_buffer.slice(..));
			render_pass.set_index_buffer(
				self.mesh_buffer.index_buffer.slice(..),
				wgpu::IndexFormat::Uint16,
			);
			render_pass.draw_indexed(0..self.mesh_buffer.num_vertices, 0, 0..1);
		}

		// self.colors[0] = [0.0, 0.0, 0.0, 0.0];

		// encoder.copy_buffer_to_buffer(
		// 	&self
		// 		.device
		// 		.create_buffer_init(&wgpu::util::BufferInitDescriptor {
		// 			label: Some("New Colors Buffer"),
		// 			contents: bytemuck::cast_slice(self.colors.as_slice()),
		// 			usage: wgpu::BufferUsages::UNIFORM | wgpu::BufferUsages::COPY_SRC,
		// 		}),
		// 	0,
		// 	&self.colors_buffer,
		// 	0,
		// 	std::mem::size_of::<[Color; 3]>() as wgpu::BufferAddress,
		// );

		self.queue.submit(std::iter::once(encoder.finish()));
		output.present();

		Ok(())
	}
}
