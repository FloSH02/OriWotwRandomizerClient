use std::collections::HashMap;

use crate::requirements::Requirement;
use crate::util::{RefillType, NodeType};

pub struct Refill {
    pub name: RefillType,
    pub requirement: Box<dyn Requirement>,
}
impl std::fmt::Debug for Refill {
    fn fmt(&self, fmt: &mut std::fmt::Formatter<'_>) -> Result<(), std::fmt::Error> {
        write!(fmt, "{:?}", self.name)
    }
}

pub struct Connection {
    pub to: String,
    pub requirement: Box<dyn Requirement>,
}
impl std::fmt::Debug for Connection {
    fn fmt(&self, fmt: &mut std::fmt::Formatter<'_>) -> Result<(), std::fmt::Error> {
        write!(fmt, "{:?}", self.to)
    }
}

#[derive(Debug)]
pub struct Anchor {
    pub identifier: String,
    pub refills: Vec<Refill>,
    pub connections: Vec<Connection>
}
#[derive(Debug)]
pub struct Pickup {
    pub identifier: String,
    pub uber_group: String,
    pub uber_id: String,
}
#[derive(Debug)]
pub struct State {
    pub identifier: String,
}
#[derive(Debug)]
pub struct Quest {
    pub identifier: String,
    pub uber_group: String,
    pub uber_id: String,
}

#[derive(Debug)]
pub enum Node {
    Anchor(Anchor),
    Pickup(Pickup),
    State(State),
    Quest(Quest),
}
impl Node {
    pub fn node_type(&self) -> NodeType {
        match self {
            Node::Anchor(_) => NodeType::Anchor,
            Node::Pickup(_) => NodeType::Pickup,
            Node::State(_) => NodeType::State,
            Node::Quest(_) => NodeType::Quest,
        }
    }
    pub fn identifier(&self) -> &str {
        match self {
            Node::Anchor(anchor) => &anchor.identifier[..],
            Node::Pickup(pickup) => &pickup.identifier[..],
            Node::State(state) => &state.identifier[..],
            Node::Quest(quest) => &quest.identifier[..],
        }
    }
    pub fn traverse(&self, player_id: u8) {
        match self {
            Node::Anchor(anchor) => {},
            Node::Pickup(pickup) => {},
            Node::State(state) => {},
            Node::Quest(quest) => {},
        }
    }
}

#[derive(Debug)]
pub struct World {
    pub graph: HashMap<String, Node>,
    pub player_id: u8,
}