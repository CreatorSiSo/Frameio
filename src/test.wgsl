struct VertexInput {
	@builtin(vertex_index) index: u32,
	@location(0) position: vec3<f32>,
	@location(1) color_id: u32,
};

struct VertexOutput {
	@builtin(position) position: vec4<f32>,
	@location(0) color_id: u32,
	@location(1) coords: vec2<f32>,
}

@group(0) @binding(0)
var<storage> colors: array<vec4<f32>, 64>;

@vertex
fn vs_main(vertex: VertexInput) -> VertexOutput {
	var out: VertexOutput;

	let x = f32(vertex.index / 2u % 2u);
	let y = f32((vertex.index - 1u) / 2u % 2u);

	out.coords = vec2<f32>(x, y);
	out.position = vec4<f32>(vertex.position, 1.0);
	out.color_id = vertex.color_id;

	return out;
}

@fragment
fn fs_debug_uv(in: VertexOutput) -> @location(0) vec4<f32> {
	return vec4<f32>(in.coords, 0.0, 1.0);
}

fn to_centered_coords(coords: vec2<f32>) -> vec2<f32> {
	return (coords - 0.5) * 2.0;
}

fn sdf_box(position: vec2<f32>, box_size: vec2<f32>) -> f32 {
	let distance = abs(position) - box_size;
	return length(max(distance, vec2<f32>(0.0))) + min(max(distance.x, distance.y), 0.0);
}

@fragment
fn fs_sdf_box(in: VertexOutput) -> @location(0) vec4<f32> {
	let sdf = sdf_box(to_centered_coords(in.coords), vec2<f32>(0.5, 0.5));

	if (sdf < 0.) { return vec4<f32>(1.0, 0.5, 0.25, 1.0); }
	return vec4<f32>(0.25, 0.5, 1.0, 1.0);
}


@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4<f32> {
	return colors[in.color_id];
}
