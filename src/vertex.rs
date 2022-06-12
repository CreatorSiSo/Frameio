/// Must be u16 or u32
/// https://gpuweb.github.io/gpuweb/#enumdef-gpuindexformat
pub type Index = u16;

#[repr(C)]
#[derive(Copy, Clone, bytemuck::Pod, bytemuck::Zeroable, Debug, Default, PartialEq)]
pub struct Vertex {
	pub position: [f32; 3],
	pub color: [f32; 4],
}

impl Vertex {
	pub fn layout<'a>() -> wgpu::VertexBufferLayout<'a> {
		wgpu::VertexBufferLayout {
			array_stride: std::mem::size_of::<Vertex>() as wgpu::BufferAddress,
			step_mode: wgpu::VertexStepMode::Vertex,
			attributes: &[
				// Position
				wgpu::VertexAttribute {
					format: wgpu::VertexFormat::Float32x3,
					offset: 0,
					shader_location: 0,
				},
				// Color
				wgpu::VertexAttribute {
					format: wgpu::VertexFormat::Float32x4,
					offset: std::mem::size_of::<[f32; 4]>() as wgpu::BufferAddress,
					shader_location: 1,
				},
			],
		}
	}
}

#[rustfmt::skip]
pub const TRI_VERTICES: &[Vertex] = &[
  // center top
	Vertex { position: [0.0, 0.8, 0.0], color: [1.0, 0.0, 0.0, 1.0] },
  // left bottom
	Vertex { position: [-0.8, -0.8, 0.0], color: [0.0, 1.0, 0.0, 1.0] },
  // right bottom
	Vertex { position: [0.8, -0.8, 0.0], color: [0.0, 0.0, 1.0, 1.0] },
];

#[rustfmt::skip]
pub const TRI_INDICES: &[Index] = &[
  0, 1, 2
];

#[rustfmt::skip]
pub const QUAD_VERTICES: &[Vertex] = &[
  // left top
	Vertex { position: [-1.0, 1.0, 0.0], color: [0.5, 0.5, 0.0, 1.0] },
  // left bottom
	Vertex { position: [-1.0, -1.0, 0.0], color: [0.0, 1.0, 0.0, 1.0] },
  // right bottom
	Vertex { position: [1.0, -1.0, 0.0], color: [0.0, 0.0, 1.0, 1.0] },
  // right top
	Vertex { position: [1.0, 1.0, 0.0], color: [0.5, 0.0, 0.5, 1.0] },
];

#[rustfmt::skip]
pub const QUAD_INDICES: &[Index] = &[
  0, 1, 2,
  3, 0, 2
];
