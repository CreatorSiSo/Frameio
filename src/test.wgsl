struct VertexInput {
	@location(0) position: vec3<f32>,
	@location(1) color_id: u32,
};

struct VertexOutput {
	@builtin(position) position: vec4<f32>,
	@location(0) color_id: u32,
};

@group(0) @binding(0)
var<uniform> colors: array<vec4<f32>, 64>;

@vertex
fn vs_main(model: VertexInput) -> VertexOutput {
	var out: VertexOutput;
	out.position = vec4<f32>(model.position, 1.0);
	out.color_id = model.color_id;
	return out;
}

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4<f32> {
	return colors[in.color_id];
}
