import time

import pimodule_base as pb 
import pimodule1 as pb1
import pimodule2 as pb2
import pimodule3 as pb3


def performance(f, args, repeat=10):
    start = time.perf_counter()
    for _ in range(repeat):
        f(*args)
    stop = time.perf_counter()
    print(f"{f.__module__}::{f.__name__} mean time ({repeat} run): {(stop-start)/repeat} s")

repeat = 10
performance(pb.compute_pi, (1000000, ), repeat )
performance(pb1.compute_pi, (1000000, ), repeat )
performance(pb2.compute_pi, (1000000, ), repeat )
performance(pb3.compute_pi, (1000000, ), repeat )