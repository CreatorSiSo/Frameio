#[repr(C)]
#[derive(Copy, Clone, Debug, bytemuck::Pod, bytemuck::Zeroable)]
pub struct Vertex {
	pub position: [f32; 3],
	pub color: [f32; 3],
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
					format: wgpu::VertexFormat::Float32x3,
					offset: std::mem::size_of::<[f32; 3]>() as wgpu::BufferAddress,
					shader_location: 1,
				},
			],
		}
	}
}
