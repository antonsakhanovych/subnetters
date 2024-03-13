mod compute;
mod parse;

use clap::Parser;
use parse::{Cli, Command};

fn main() {
    let args = Cli::parse();
    if let Command::Run {
        ip,
        cidr_mask,
        sizes,
    } = args.command
    {
        println!("IP {} with mask {}", ip, cidr_mask);
        println!("{:?}", sizes);
    }
}
