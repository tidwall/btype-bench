extern crate rand;

use rand::Rng;
use std::collections::BTreeSet;
use std::time::Instant;
use num_format::{Locale, ToFormattedString};
use std::io::Write;

const N:usize = 1000000;
const R:usize = 50;

trait KeyType: Ord + Default {
    fn make_key(&mut self, x: usize);
    fn name(&self) -> &str;
}

// Implement it for specific primitives
impl KeyType for i32 {
    fn make_key(&mut self, x: usize) { *self = x as Self }
    fn name(&self) -> &str { "int32" }
}
impl KeyType for u64 {
    fn make_key(&mut self, x: usize) { *self = x as Self }
    fn name(&self) -> &str { "uint64" }
}
impl KeyType for String {
    fn make_key(&mut self, x: usize) { *self = x.to_string() }
    fn name(&self) -> &str { "string" }
}

fn make_key<T: KeyType + Default>(x: usize) -> T {
    let mut out = T::default();
    out.make_key(x);
    out
}

fn bench_print(n:usize, elapsed:f64) {
    print!("{:>9} ops in ", n.to_formatted_string(&Locale::en));
    print!("{:7.3} secs ", elapsed);
    print!("{:8.1} ns/op ", elapsed*1e9/(n as f64));
    print!("{:>13} op/sec", 
        (((n as f64)/elapsed) as i64).to_formatted_string(&Locale::en));
    println!();
}

fn run_op<T: KeyType>(label: &str, n:usize, r:usize, db: &mut BTreeSet<T>, 
    keys: &mut Vec<T>, 
    pre: &dyn Fn(&mut BTreeSet<T>, &mut Vec<T>),
    op: &dyn Fn(&mut BTreeSet<T>, &mut Vec<T>))
{
    let mut felapsed = Vec::<u64>::new();
    for i in 0..r {
        print!("\r{:19}{}/{} ", label, i, r);
        std::io::stdout().flush().unwrap();
        pre(db, keys);
        let start = Instant::now();
        op(db, keys);
        let elapsed = start.elapsed().as_secs_f64()*1000000.0;
        felapsed.push(elapsed as u64)
    }
    print!("\r{:19}", label);
    felapsed.sort();
    bench_print(n, (felapsed[r/2] as f64)/1000000.0);
}

fn shuffle<T>(data: &mut Vec<T>) {
    rand::thread_rng().shuffle(data);
}

fn fill_keys<T: KeyType>(n: usize, keys: &mut Vec<T>) {
    let mut perm: Vec<usize> = (0..n).collect();
    shuffle(&mut perm);
    keys.clear();
    for i in 0..perm.len() {
        keys.push(make_key(perm[i]));
    }
}

fn bench<T: KeyType>() {
    let n = std::env::var("N").unwrap_or("".to_string()).parse().unwrap_or(N);
    let r = std::env::var("R").unwrap_or("".to_string()).parse().unwrap_or(R);
    println!("Benchmarking {} items, {} keys, {} times, using {}", 
        n, T::default().name(), r, "median");
    let mut db = BTreeSet::<T>::new();
    let mut keys = Vec::<T>::new();
    run_op("insert(seq)", n, r, &mut db, &mut keys, 
        &|db, keys|{
            db.clear();
            fill_keys(n, keys);
            keys.sort();
            keys.reverse(); // needed for popping in sorted order
        },
        &|db, keys|{
            for _ in 0..n {
                assert!(db.insert(keys.pop().unwrap()));
            }
        }
    );
    run_op("insert(rand)", n, r, &mut db, &mut keys, 
        &|db, keys|{
            db.clear();
            fill_keys(n, keys);
            shuffle(keys);
        },
        &|db, keys|{
            for _ in 0..n {
                assert!(db.insert(keys.pop().unwrap()));
            }
        }
    );
    run_op("get(seq)", n, r, &mut db, &mut keys, 
        &|_, keys|{
            fill_keys(n, keys);
            keys.sort();
        },
        &|db, keys|{
            for i in 0..n {
                assert!(db.contains(&keys[i]));
            }
        }
    );
    run_op("get(rand)", n, r, &mut db, &mut keys, 
        &|_, keys|{
            fill_keys(n, keys);
            shuffle(keys);
        },
        &|db, keys|{
            for i in 0..n {
                assert!(db.contains(&keys[i]));
            }
        }
    );
}

fn main() {
    bench::<i32>();
    bench::<u64>();
    bench::<String>();
}
