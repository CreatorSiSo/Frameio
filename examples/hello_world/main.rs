use appio::App;

fn main() {
	pollster::block_on(async {
		env_logger::init();

		let mut app = App::default();

		app.spawn_window("one").await;
		app.spawn_window("two").await;
		app.run();
	})
}
