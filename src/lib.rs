pub fn print_hello_world() {
	println!("Hello World!");
}

#[cfg(test)]
mod tests {
	use super::*;

	#[test]
	fn it_works() {
		let result = 2 + 2;
		print_hello_world();
		assert_eq!(result, 4);
	}
}
