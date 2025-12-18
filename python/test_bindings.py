#!/usr/bin/env python3
"""
Test script for pykm2arec bindings
"""

import sys
import os

# Add build directory to path if running from source
build_dir = os.path.join(os.path.dirname(__file__), '..', 'build', 'python')
if os.path.exists(build_dir):
    sys.path.insert(0, build_dir)

try:
    import pykm2arec
except ImportError as e:
    print(f"Failed to import pykm2arec: {e}")
    print("Make sure you have built the project first.")
    sys.exit(1)


def test_basic():
    """Basic functionality test"""
    print("=== Testing pykm2arec bindings ===\n")
    
    # Test file path
    test_file = "../test/KM2AMCTest.root"
    if not os.path.exists(test_file):
        test_file = os.path.join(os.path.dirname(__file__), '..', 'test', 'KM2AMCTest.root')
    
    print(f"Opening file: {test_file}")
    source = pykm2arec.KM2AEventSource(test_file)
    
    print(f"Source: {source}")
    print(f"Number of events: {len(source)}")
    print(f"event_info entries: {source.size_event_info()}")
    print()
    
    # Test indexing
    print("=== Test indexing ===")
    event = source[0]
    print(f"Event 0: {event}")
    print(f"  eve_id: {event.eve_id}")
    print(f"  eve_ene: {event.eve_ene}")
    print(f"  eve_corex: {event.eve_corex}")
    print(f"  eve_corey: {event.eve_corey}")
    
    if event.lhevent:
        lh = event.lhevent
        print(f"  LHEvent: {lh}")
        print(f"    Energy: {lh.energy}")
        print(f"    Theta: {lh.theta}")
        print(f"    Phi: {lh.phi}")
        print(f"    NhitE: {lh.nhit_e}")
        print(f"    NhitM: {lh.nhit_m}")
        
        # Test hit access
        if lh.nhit_e > 0:
            hit = event.GetHitE(0)
            if hit:
                print(f"    First HitE: {hit}")
    print()
    
    # Test negative indexing
    print("=== Test negative indexing ===")
    last_event = source[-1]
    print(f"Last event: {last_event}")
    print()
    
    # Test iteration (first 3 events)
    print("=== Test iteration (first 3 events) ===")
    for i, event in enumerate(source):
        if i >= 3:
            break
        print(f"Event {i}: eve_id={event.eve_id}, eve_ene={event.eve_ene:.2f}")
        if event.lhevent:
            print(f"         E={event.lhevent.energy:.2f}, NhitE={event.lhevent.nhit_e}")
    print()
    
    print("=== All tests passed! ===")


if __name__ == "__main__":
    test_basic()

