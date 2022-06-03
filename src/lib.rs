// use std::collections::HashMap;

// pub struct Tree<NodeId, Node = (), Link = ()> {
// 	nodes: HashMap<NodeId, Node, Link>,
// }

// #[derive(Debug, Default)]
// pub struct Node<T = ()> {
// 	pub data: T,
// 	pub children: Vec<Node<T>>,
// }

// impl<T> Node<T> {
// 	pub fn new(data: T) -> Self {
// 		Self {
// 			data,
// 			children: Vec::default(),
// 		}
// 	}
// }

#[derive(Debug, Default, PartialEq)]
pub struct Element {
	pub name: String,
	pub children: Vec<Element>,
}

impl Element {
	pub fn new(name: &str) -> Self {
		Self {
			name: name.into(),
			children: Vec::default(),
		}
	}

	pub fn insert_child(&mut self, child: Element) -> &mut Element {
		self.children.push(child);
		self
			.children
			.last_mut()
			.expect("Just inserted child Element does not exist!")
	}

	pub fn insert_children(&mut self, mut children: Vec<Element>) -> &mut Self {
		self.children.append(&mut children);
		self
	}

	pub fn iter(&self) -> ElementIter<'_> {
		ElementIter(self.children.as_slice())
	}

	pub fn iter_mut(&mut self) -> ElementIterMut<'_> {
		ElementIterMut(self.children.as_mut_slice())
	}

	pub fn _build() {
		todo!()
	}
}

pub struct ElementIter<'a>(&'a [Element]);

impl<'a> Iterator for ElementIter<'a> {
	type Item = &'a Element;

	fn next(&mut self) -> Option<Self::Item> {
		match self.0.split_first() {
			Some((first, rest)) => {
				self.0 = rest;
				Some(first)
			}
			None => None,
		}
	}
}

impl<'a> IntoIterator for &'a Element {
	type Item = &'a Element;

	type IntoIter = ElementIter<'a>;

	fn into_iter(self) -> Self::IntoIter {
		self.iter()
	}
}

pub struct ElementIterMut<'a>(&'a mut [Element]);

impl<'a> Iterator for ElementIterMut<'a> {
	type Item = &'a mut Element;

	fn next(&mut self) -> Option<Self::Item> {
		// Does not work without swap
		// https://stackoverflow.com/questions/61847200/cannot-infer-an-appropriate-lifetime-for-autoref-due-to-conflicting-requirement
		let mut temp: ElementIterMut<'a> = ElementIterMut(&mut []);
		std::mem::swap(&mut self.0, &mut temp.0);

		match temp.0.split_first_mut() {
			Some((first, rest)) => {
				self.0 = rest;
				Some(first)
			}
			None => None,
		}
	}
}

impl<'a> IntoIterator for &'a mut Element {
	type Item = &'a mut Element;

	type IntoIter = ElementIterMut<'a>;

	fn into_iter(self) -> Self::IntoIter {
		self.iter_mut()
	}
}

#[test]
fn test_borrowing_for_loop() {
	let mut root = Element::new("Root");
	root.insert_children(vec![
		Element::new("GlobalMenu"),
		Element::new("Editor"),
		Element::new("Editor"),
		Element::new("Editor"),
	]);

	for child in &root {
		let temp = child;
		println!("{:#?}", temp);
	}
}

#[test]
fn test_mutable_for_loop() {
	let mut root = Element::new("Root");
	root.insert_children(vec![
		Element::new("GlobalMenu"),
		Element::new("Editor"),
		Element::new("Editor"),
		Element::new("Editor"),
	]);

	for child in &mut root {
		child.name = "new_name".into()
	}

	println!("{:#?}", root);
}
