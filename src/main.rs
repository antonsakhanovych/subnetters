mod parse;
use parse::parse_args;

fn main() {
    let config = parse_args().unwrap();
}
