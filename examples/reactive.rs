// fn closure_user(closure: Box<dyn FnOnce() -> Box<dyn std::any::Any>>) {
// 	closure();
// }

// fn main() {
// 	closure_user(Box::new(|| Box::new(false)));
// }

use std::{any::Any, error::Error};

fn record(storage: &mut Vec<fn(i32, i32) -> dyn Any>, closure: fn(i32, i32) -> dyn Any) {
	storage.push(closure)
}

fn replay(storage: &Vec<Box<impl Fn() -> Box<dyn Any>>>) {
	for closure in storage {
		closure();
	}
}

fn main() -> Result<(), Box<dyn Error>> {
	let mut storage: Vec<_> = Vec::default();

	let mut one = 3490;
	let mut two = 4936;
	let mut total = 0;

	fn calc_total(one: i32, two: i32) -> i32 {
		one + two
	}

	// record(&mut storage, calc_total);

	// calc_total();

	Ok(())
}

// const storage: Array<() => any> = []

// function record(func: () => any) { storage.push(func) }
// function replay() {
//   for (let func of storage) {
//     func()
//   }
// }

// let one = 3490
// let two = 4936
// let total

// const target = () => { total = one + two }
// record(target)

// target()
// console.log(one, two, total)

// one = 0
// two = 49

// replay()
// console.log(one, two, total)

// use std::{error::Error, rc::Rc};

// fn main() -> Result<(), Box<dyn Error>> {
// 	let mut rc_one = Rc::from(32);
// 	let mut rc_two = Rc::from(43);

// 	println!("{} {}", Rc::as_ref(&rc_one), Rc::as_ref(&rc_two));

// 	*Rc::get_mut(&mut rc_one).unwrap() = 20;
// 	*Rc::get_mut(&mut rc_two).unwrap() = 20;

// 	println!("{} {}", Rc::as_ref(&rc_one), Rc::as_ref(&rc_two));

// 	Ok(())
// }
