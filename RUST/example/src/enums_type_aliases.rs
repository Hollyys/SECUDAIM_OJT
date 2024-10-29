enum VeryVerboseEnumOfThingsToDoWithNumbers {
    Add,
    Subtract,
}

// Creates a type alias
type Operations = VeryVerboseEnumOfThingsToDoWithNumbers;

// enum VeryVerboseEnumOfThingsToDoWithNumber_2 {
//     Add,
//     Subtract,
// }

// impl VeryVerboseEnumOfThingsToDoWithNumbers_2 {
//     fn run(&self, x: i32, y: i32) -> i32 {
//         match self {
//             Self::Add => x + y,
//             Self::Subtract => x - y,
//         }
//     }
// }

#[allow(unused_variables)]
fn main() {
    // We can refer to each variant via its alias, not its long and inconvenient
    // name.
    let x = Operations::Add;
}
