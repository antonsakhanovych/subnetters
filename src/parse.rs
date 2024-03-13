use std::net::IpAddr;

use clap::{Parser, Subcommand};

#[derive(Debug, Parser)]
#[command(version, about, long_about = None)]
pub struct Cli {
    #[command(subcommand)]
    pub command: Command,
}

#[derive(Debug, Subcommand)]
pub enum Command {
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
