# Appio

Component based ui-framework for Rust inspired by Vue.

## Goals

- Components
  - [ ] Encapsulated state
  - [ ] Only reeveluate if state has changed
  - [ ] General styling api
  - [ ] Component creation
    - [ ] Minimal boilerplate
    - [ ] Combine other components into a new one
- Events
  - [ ] Get propagated up to the trees root and back down to all leaves
  - [ ] Event propagation can be stopped when it has been handled
  - [ ] Create and send custom events
- Layout (CPU)
  1. [ ] Calculate bounding boxes for all components
  2. [ ] Generate clip_nodes
     - [ ] clip_path
       - [ ] overflow:
         - visible: `clip_path == viewport` (just do not create the clip_node)
         - hidden: `clip_path == bounding_box`
         - hidden-x: `clip_path.x == bounding_box.x, clip_path.y == viewport.y`
         - hidden-y: `clip_path.x == viewport.x, clip_path.y == bounding_box.y`
       - [ ] custom clip_path shape?
- Rendering (GPU)
  - [ ] Vertex shader
    - [ ] Transformations: Rotation, Translation, Scaling (might have to be done before submitting to the GPU because it changes the bounding_box)
  - [ ] Fragment shader
    - [ ] Node inside of clip_path -> Alpha = 1
    - [ ] Node on clip_path -> calculate Alpha for every pixel
    - [ ] Node outside of clip_path -> Alpha = 0
    - [ ] Shaders
      - [ ] background/fill, text (sdf or path), border, border_radii, etc.
      - [ ] https://madebyevan.com/shaders/fast-rounded-rectangle-shadows/
  - [ ] Execute shaders for each render-node
- Composition (GPU)
  - [ ] Combine rendered textures of each clip_node into one for the entire viewport
  - [ ] Viewport wide effects?

## Todo

- transperancy
- proper errors for renderer new function
- colors lookup
  - make dynamically updatable/resizable
  - push new colors to it when a new quad is created
- [ ] API for drawing
  - [ ] get surface/context
  - [ ] draw_quad()
  - [ ] draw_quad_rounded()

## Structure

```js

Renderer {
	variables: [
		device,
		queue,
		surface, // maybe support multiple surfaces
		surface_config,
		render_pipline?, // break up even more later to use multiple shaders
		data_changed,
	],
	methods: {
		new() => Self {
			create interface; // https://gpuweb.github.io/gpuweb/#gpu-interface
			create surface;
			create adapter; // https://gpuweb.github.io/gpuweb/#gpu-adapter
			create device, queue;
			set initial surface size;
			create surface_config;

			return Self {
				device,
				queue,
				surface,
				surface_config,
				render_pipline: None
				should_rerender: true,
			}
		},

		configure_pipline(&mut self) => Option<Error> {
			create frag_bind_group_layout;
			create frag_bind_group;
			create render_pipeline_layout;
			compile shader;
			create render_pipeline_descriptor;
			create render_pipeline;
		}

		set_mouse_position(&mut self) {}

		update(&mut self) {
			if some_condition {
				create new_batched_mesh;
				self.mesh_buffer = new_batched_mesh;
			}
		}

		render(&self) {
			create output;

			if self.data_changed {
				create frame_buffer;

				self.data_changed = false;
			}

			output.present();
		}
	}
}

```
