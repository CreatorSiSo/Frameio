use crate::vertex::*;

#[derive(Debug)]
pub struct MeshBuffer {
	pub num_vertices: u32,
	pub vertex_buffer: wgpu::Buffer,
	pub index_buffer: wgpu::Buffer,
}

#[derive(Debug, Default, PartialEq)]
pub struct Mesh {
	vertices: Vec<Vertex>,
	indices: Vec<Index>,
}

impl Mesh {
	/// Combines multiple meshes into one batched mesh.
	pub fn new_batched(meshes: Vec<Mesh>) -> Mesh {
		Mesh {
			vertices: meshes
				.iter()
				.flat_map(|mesh| mesh.vertices.iter().cloned())
				.collect(),
			indices: {
				let first_length = meshes[0].vertices.len();
				meshes
					.iter()
					// TODO: Maybe throw an error if casting len() -> usize to a u16 or u32 results in a wrap around
					// This will be fine if the mesh does not have over 65535 (u16) or 4294967295 (u32) vertices
					.map(|mesh| (&mesh.indices, (mesh.vertices.len() - first_length) as Index))
					.flat_map(|(indices, offset)| indices.into_iter().map(move |index| index + offset))
					.collect()
			},
		}
	}
}

#[cfg(test)]
mod test {
	use super::*;

	#[test]
	fn test_new_batched() {
		let mesh_red = Mesh {
			#[rustfmt::skip]
			vertices: vec![
				Vertex { position: [-0.5, 0.5, 0.0], color: [1.0, 0.0, 0.0, 1.0] },
				Vertex { position: [-0.5, -0.5, 0.0], color: [1.0, 0.0, 0.0, 1.0] },
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
				Vertex { position: [-1.0, 1.0, 0.0], color: [0.0, 0.0, 1.0, 1.0] },
				Vertex { position: [-1.0, -1.0, 0.0], color: [0.0, 0.0, 1.0, 1.0] },
				Vertex { position: [1.0, -1.0, 0.0], color: [0.0, 0.0, 1.0, 1.0] },
				Vertex { position: [1.0, 1.0, 0.0], color: [0.0, 0.0, 1.0, 1.0] },
			],
			#[rustfmt::skip]
			indices: vec![
				0, 1, 2,
				3, 0, 2,
			],
		};

		let batched_mesh = Mesh::new_batched(vec![mesh_red, mesh_blue]);

		assert_eq!(
			batched_mesh,
			Mesh {
				#[rustfmt::skip]
				vertices: vec![
					// Red
					Vertex { position: [-0.5, 0.5, 0.0], color: [1.0, 0.0, 0.0, 1.0] },
					Vertex { position: [-0.5, -0.5, 0.0], color: [1.0, 0.0, 0.0, 1.0] },

					// Blue
					Vertex { position: [-1.0, 1.0, 0.0], color: [0.0, 0.0, 1.0, 1.0] },
					Vertex { position: [-1.0, -1.0, 0.0], color: [0.0, 0.0, 1.0, 1.0] },
					Vertex { position: [1.0, -1.0, 0.0], color: [0.0, 0.0, 1.0, 1.0] },
					Vertex { position: [1.0, 1.0, 0.0], color: [0.0, 0.0, 1.0, 1.0] },
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
			}
		)
	}
}
