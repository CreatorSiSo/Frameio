struct VertexInput {
	@builtin(vertex_index) index: u32,
	@location(0) position: vec3<f32>,
	@location(1) color_id: u32,
};

struct VertexOutput {
	@builtin(position) position: vec4<f32>,
	@location(0) color_id: u32,
	@location(1) uv_coords: vec2<f32>,
}

@group(0) @binding(0)
var<uniform> colors: array<vec4<f32>, 64>;

@vertex
fn vs_main(vertex: VertexInput) -> VertexOutput {
	var out: VertexOutput;

	let u = f32(vertex.index / 2u % 2u);
	let v = f32((vertex.index - 1u) / 2u % 2u);

	out.uv_coords = vec2<f32>(u, v);
	out.position = vec4<f32>(vertex.position, 1.0);
	out.color_id = vertex.color_id;

	return out;
}

@fragment
fn fs_debug_uv(in: VertexOutput) -> @location(0) vec4<f32> {
	return vec4<f32>(in.uv_coords, 0.0, 1.0);
}


@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4<f32> {
	return colors[in.color_id];
}
