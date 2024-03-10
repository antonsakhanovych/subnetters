use std::net::IpAddr;

use clap::{Parser, Subcommand};

#[derive(Debug, Parser)]
#[command(version, about, long_about = None)]
struct Cli {
    #[command(subcommand)]
    command: Command,
}

#[derive(Debug, Subcommand)]
enum Command {
    #[command(arg_required_else_help = true)]
    Run {
        #[arg(required = true)]
        ip: IpAddr,
        #[arg(required = true)]
        cidr_mask: u8,
        #[arg(required = true)]
        sizes: Vec<u128>,
    },
    // Tui, // will be implemented later
}

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
