use appio::App;

fn main() {
	pollster::block_on(async {
		env_logger::init();

		let mut app = App::new("Hello World").await;

		app.run();
	})
}
