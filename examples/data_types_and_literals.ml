
let x: i32 = 42;
let pi: f64 = 3.14159;

let message: str = "Hello, My Language!";
let grade: char = 'A';

cls MyClass {
  pub name: char;

  pub fn init(x: i64) {
    let a: i64 = x;
  }

  pub fn add(x: i64, y: i64): i64 {
    return x + y;
  }
}

let b: MyClass = MyClass(1);
let a: i64 = b.add(1.1230, 2);

while (true) {
  break;
}

break;