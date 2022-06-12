use crate::{color::*, vertex::*};

#[derive(Debug)]
pub struct MeshBuffer {
	pub num_vertices: u32,
	pub vertex_buffer: wgpu::Buffer,
	pub index_buffer: wgpu::Buffer,
}

#[derive(Clone, Debug, Default, PartialEq)]
pub struct Mesh {
	pub vertices: Vec<Vertex>,
	pub indices: Vec<Index>,
}

impl Mesh {
	/// Creates a new 2d quad mesh.
	pub fn new_quad(width: f32, height: f32, color_id: ColorId) -> Self {
		Mesh {
			#[rustfmt::skip]
			vertices: vec![
				// left top
				Vertex { position: [0.0,   height, 0.], color_id },
				// left bottom
				Vertex { position: [0.0,   0.0,    0.], color_id },
				// right bottom
				Vertex { position: [width, 0.0,    0.], color_id },
				// right top
				Vertex { position: [width, height, 0.], color_id },
			],
			#[rustfmt::skip]
			indices: vec![
				0, 1, 2,
				3, 0, 2,
			],
		}
	}

	/// Combines multiple meshes into one batched mesh.
	pub fn make_batched(meshes: Vec<Mesh>) -> Mesh {
		meshes
			.iter()
			.cloned()
			.fold(Mesh::default(), |mut combined, mut current| {
				combined.indices.append(
					&mut current
						.indices
						.iter()
						.map(|index: &Index| index + combined.vertices.len() as Index)
						.collect::<Vec<Index>>(),
				);
				combined.vertices.append(&mut current.vertices);
				combined
			})
	}
}

#[cfg(test)]
mod test {
	use super::*;

	#[test]
	fn test_make_batched() {
		let mesh_red = Mesh {
			#[rustfmt::skip]
			vertices: vec![
				Vertex { position: [-0.5, 0.5, 0.], color_id: 0 },
				Vertex { position: [-0.5, -0.5, 0.], color_id: 0 },
			],
			#[rustfmt::skip]
			indices: vec![
				0, 1,
				1, 0,
			],
		};

		let mesh_blue = Mesh {
			#[rustfmt::skip]
			vertices: vec![
				Vertex { position: [-1.0, 1.0, 0.], color_id: 0 },
				Vertex { position: [-1.0, -1.0, 0.], color_id: 0 },
				Vertex { position: [1.0, -1.0, 0.], color_id: 0 },
				Vertex { position: [1.0, 1.0, 0.], color_id: 0 },
			],
			#[rustfmt::skip]
			indices: vec![
				0, 1, 2,
				3, 0, 2,
			],
		};

		let expected_mesh = Mesh {
			#[rustfmt::skip]
			vertices: vec![
				// Red
				Vertex { position: [-0.5, 0.5, 0.], color_id: 0 },
				Vertex { position: [-0.5, -0.5, 0.], color_id: 0 },

				// Blue
				Vertex { position: [-1.0, 1.0, 0.], color_id: 0 },
				Vertex { position: [-1.0, -1.0, 0.], color_id: 0 },
				Vertex { position: [1.0, -1.0, 0.], color_id: 0 },
				Vertex { position: [1.0, 1.0, 0.], color_id: 0 },
			],
			#[rustfmt::skip]
			indices: vec![
				// Red
					0, 1,
					1, 0,

				// Blue
					2, 3, 4,
					5, 2, 4,
			],
		};

		assert_eq!(Mesh::make_batched(vec![mesh_red, mesh_blue]), expected_mesh);
	}

	#[test]
	fn test_make_batched_quads() {
		let quad_mesh = Mesh::new_quad(3.0, 2.0, 0);

		let batched_mesh = Mesh::make_batched(vec![quad_mesh.clone(), quad_mesh.clone(), quad_mesh]);

		let expected_mesh = Mesh {
			#[rustfmt::skip]
			vertices: vec![
				Vertex { position: [0.0, 2.0, 0.], color_id: 0 },
				Vertex { position: [0.0, 0.0, 0.], color_id: 0 },
				Vertex { position: [3.0, 0.0, 0.], color_id: 0 },
				Vertex { position: [3.0, 2.0, 0.], color_id: 0 },

				Vertex { position: [0.0, 2.0, 0.], color_id: 0 },
				Vertex { position: [0.0, 0.0, 0.], color_id: 0 },
				Vertex { position: [3.0, 0.0, 0.], color_id: 0 },
				Vertex { position: [3.0, 2.0, 0.], color_id: 0 },

				Vertex { position: [0.0, 2.0, 0.], color_id: 0 },
				Vertex { position: [0.0, 0.0, 0.], color_id: 0 },
				Vertex { position: [3.0, 0.0, 0.], color_id: 0 },
				Vertex { position: [3.0, 2.0, 0.], color_id: 0 },
			],
			#[rustfmt::skip]
			indices: vec![
				0, 1, 2,
				3, 0, 2,

				4, 5, 6,
				7, 4, 6,

				8, 9, 10,
				11, 8, 10,
			],
		};

		assert_eq!(batched_mesh, expected_mesh);
	}

	#[test]
	fn test_new_quad() {
		let quad_mesh = Mesh::new_quad(3.0, 2.0, 0);

		assert_eq!(
			quad_mesh,
			Mesh {
				#[rustfmt::skip]
				vertices: vec![
					Vertex { position: [0.0, 2.0, 0.], color_id: 0 },
					Vertex { position: [0.0, 0.0, 0.], color_id: 0 },
					Vertex { position: [3.0, 0.0, 0.], color_id: 0 },
					Vertex { position: [3.0, 2.0, 0.], color_id: 0 },
				],
				#[rustfmt::skip]
				indices: vec![
					0, 1, 2,
					3, 0, 2,
				],
			}
		)
	}
}

// #[rustfmt::skip]
// pub const TRI_VERTICES: &[Vertex] = &[
//   // center top
// 	Vertex { position: [0.0, 0.8, 0.0], color: [1.0, 0.0, 0.0, 1.0] },
//   // left bottom
// 	Vertex { position: [-0.8, -0.8, 0.0], color: [0.0, 1.0, 0.0, 1.0] },
//   // right bottom
// 	Vertex { position: [0.8, -0.8, 0.0], color: [0.0, 0.0, 1.0, 1.0] },
// ];

// #[rustfmt::skip]
// pub const TRI_INDICES: &[Index] = &[
//   0, 1, 2
// ];

// #[rustfmt::skip]
// pub const QUAD_VERTICES: &[Vertex] = &[
//   // left top
// 	Vertex { position: [-1.0, 1.0, 0.0], color: [0.5, 0.5, 0.0, 1.0] },
//   // left bottom
// 	Vertex { position: [-1.0, -1.0, 0.0], color: [0.0, 1.0, 0.0, 1.0] },
//   // right bottom
// 	Vertex { position: [1.0, -1.0, 0.0], color: [0.0, 0.0, 1.0, 1.0] },
//   // right top
// 	Vertex { position: [1.0, 1.0, 0.0], color: [0.5, 0.0, 0.5, 1.0] },
// ];

// #[rustfmt::skip]
// pub const QUAD_INDICES: &[Index] = &[
//   0, 1, 2,
//   3, 0, 2
// ];
