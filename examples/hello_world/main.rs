use appio::prelude::*;

fn main() {
	let mut root = Element::new("Root");

	root
		.insert_child(Element::new("Viewport"))
		.insert_children(vec![
			Element::new("GlobalMenu"),
			Element::new("Editor"),
			Element::new("Editor"),
			Element::new("Editor"),
		]);

	for child in &root.children[0] {
		println!("{:#?}", child);
		println!("-----");
	}

	for child in &mut root.children[0] {
		child.insert_child(Element::new("test"));
	}

	println!("{:#?}", root);
}
