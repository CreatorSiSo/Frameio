struct VertexOutput {
    @builtin(position) clip_position: vec4<f32>,
    @location(0) position: vec2<f32>,
}

@vertex
fn vs_main(
    @builtin(vertex_index) in_vertex_index: u32,
) -> VertexOutput {
    var out: VertexOutput;
    let x = f32(1 - i32(in_vertex_index));
    let y = f32(i32(in_vertex_index & 1u) * 2 - 1);

    out.position = vec2<f32>(x, y);
    out.clip_position = vec4<f32>(out.position * 0.8, 0.0, 1.0);

    return out;
}

@fragment
fn fs_main_pos(in: VertexOutput) -> @location(0) vec4<f32> {
    return vec4<f32>(in.position, 0.0, 1.0);
}

@fragment
fn fs_main_col(in: VertexOutput) -> @location(0) vec4<f32> {
    return vec4<f32>(0.3, 0.2, 0.1, 1.0);
}
