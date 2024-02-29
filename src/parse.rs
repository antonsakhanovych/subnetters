use std::{env, net::IpAddr};

#[derive(Debug)]
pub struct Config {
    net_addr: IpAddr,
    cidr: u8,
    sizes: Vec<u128>,
}

#[derive(Debug)]
pub enum ArgsParsingError {
    IpAddressExpected,
    CidrTooBig,
}

pub fn parse_args() -> Result<Config, ArgsParsingError> {
    let mut args = env::args();
    let _ = args.next();
    let net_addr: IpAddr = args.next().unwrap().parse().unwrap();
    let cidr_max_size = match net_addr {
        IpAddr::V4(_) => 32,
        IpAddr::V6(_) => 128,
    };
    let cidr: u8 = args.next().unwrap().parse().unwrap();
    if cidr > cidr_max_size {
        return Err(ArgsParsingError::CidrTooBig);
    }

    let sizes = args.filter_map(|num| num.parse::<u128>().ok()).collect();
    Ok(Config {
        net_addr,
        cidr,
        sizes,
    })
}
