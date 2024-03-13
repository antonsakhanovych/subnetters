use std::{collections::VecDeque, net::IpAddr};

pub const IPV4_BITS: u8 = 32;
pub const IPV6_BITS: u8 = 128;

fn get_subnet_power(num_hosts: u128) -> u8 {
    let mut power: u8 = 0;
    let mut num_hosts = num_hosts + 2;
    while num_hosts + 2 > 0 {
        power += 1;
        num_hosts >>= 1;
    }
    power
}

#[derive(Debug, Clone)]
pub struct Subnet {
    ip: IpAddr,
    cidr_mask: u8,
}

impl Subnet {
    // Returns None if cidr_mask violates ip rules
    pub fn new(ip: IpAddr, cidr_mask: u8) -> Option<Self> {
        let max_mask = match ip {
            IpAddr::V4(_) => IPV4_BITS,
            IpAddr::V6(_) => IPV6_BITS,
        };
        if cidr_mask > max_mask {
            None
        } else {
            Some(Self { ip, cidr_mask })
        }
    }
    fn calculate_size(&self) -> u128 {
        (match self.ip {
            IpAddr::V4(_) => IPV4_BITS,
            IpAddr::V6(_) => IPV6_BITS,
        } - self.cidr_mask) as u128
    }
    fn generate_mask(&self) -> IpAddr {
        match self.ip {
            IpAddr::V4(_) => {
                IpAddr::V4((u32::MAX - ((1 << (IPV4_BITS - self.cidr_mask)) - 1)).into())
            }

            IpAddr::V6(_) => {
                IpAddr::V6((u128::MAX - ((1 << (IPV6_BITS - self.cidr_mask)) - 1)).into())
            }
        }
    }
}

struct Computer {
    subnet: Subnet,
    sizes: VecDeque<u128>,
}

impl Computer {
    pub fn new(subnet: Subnet, sizes: Vec<u128>) -> Self {
        let mut sizes = sizes;

        sizes.sort();
        sizes.reverse();
        Self {
            subnet,
            sizes: VecDeque::from(sizes),
        }
    }
}

impl Iterator for Computer {
    type Item = Subnet;
    fn next(&mut self) -> Option<Self::Item> {
        if let Some(net_size) = self.sizes.pop_front() {
            if net_size + 2 < self.subnet.calculate_size() {
                return None;
            }
            let power = get_subnet_power(net_size);
        }
        None
    }
}
