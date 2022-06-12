use crate::color::ColorId;

/// Must be u16 or u32
/// https://gpuweb.github.io/gpuweb/#enumdef-gpuindexformat
pub type Index = u16;

#[repr(C)]
#[derive(Copy, Clone, bytemuck::Pod, bytemuck::Zeroable, Debug, Default, PartialEq)]
pub struct Vertex {
	pub position: [f32; 3],
	pub color_id: ColorId,
}

impl Vertex {
	const ATTRIBUTES: [wgpu::VertexAttribute; 2] =
		wgpu::vertex_attr_array![0 => Float32x3, 1 => Uint32];

	pub fn layout<'a>() -> wgpu::VertexBufferLayout<'a> {
		wgpu::VertexBufferLayout {
			array_stride: std::mem::size_of::<Vertex>() as wgpu::BufferAddress,
			step_mode: wgpu::VertexStepMode::Vertex,
			attributes: &Self::ATTRIBUTES,
		}
	}
}
