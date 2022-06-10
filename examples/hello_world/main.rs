use appio::prelude::*;

fn main() {
	pollster::block_on(async {
		env_logger::init();

		App::new("Hello World").await.run();
	})
}
