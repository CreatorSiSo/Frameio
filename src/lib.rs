#![doc = include_str!("../readme.md")]

pub mod utils;
pub mod prelude {
	pub use crate::app::App;
	pub use crate::utils::default;
}

mod app;
mod color;
mod mesh;
mod renderer;
mod vertex;
