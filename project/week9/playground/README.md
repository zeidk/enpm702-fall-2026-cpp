# Week 9 playground

The code of the Lecture 9 slides, so you can run it, change it and run it again.
Each block of code is tagged with the frame it comes from: `[Slide 75]` is the
number in the top-left corner of a main-deck slide, and `[Appendix xl]` is an
appendix frame.

**Not all of this code compiles.** Some slides show code that the compiler
rejects, because the error is the lesson. That code is in `not_compiling/`,
outside every build: `702build` never compiles it, and the error each file gives
is on its slide and in the table below.

## Run a slide's code

```bash
702build week9_playground
702run week9_playground        # every slide, then every appendix frame
702run week9_playground 75     # only [Slide 75]
702run week9_playground xl     # only [Appendix xl]
```

[Slide 64] deletes a `RoboTaxi` through a `Vehicle` pointer with no virtual
destructor: undefined behavior. A full run skips it; `702run week9_playground 64`
runs it alone, and AddressSanitizer stops it with `new-delete-type-mismatch`.

## Folders

| Folder | What it holds |
|---|---|
| `include/`, `src/` | The program `week9_playground`: one file per section of the deck (`relationships.cpp`, `inheritance.cpp`, `polymorphism.cpp`, `abstraction.cpp`), `appendix.cpp` for the appendix, `undefined.cpp` for [Slide 64], and the finished classes of the class diagram (`Vehicle`, `RoboTaxi`, `Taxi`, `Fleet`, `Passenger`, `Sensor`, `Route`, `Position`). |
| `operators/` | The program of the reading page Operator Overloading on Read the Docs: `702run week9_operators`, or `702run week9_operators print` for one part. |
| `cycle/` | Two classes that refer to each other, built with forward declarations ([Appendix xxxix]): `702run week9_cycle`. |
| `not_compiling/` | The code of the slides that do not compile, one file per frame. |
| `uml/` | The PlantUML source of the UML diagrams on the slides. |

## Code that does not compile

Each file is self-contained. To see its error, from `not_compiling/`:

```bash
g++ -std=c++20 -fsyntax-only -I../include slide_065_not_the_other_way.cpp
```

For the cycle, compile `appendix_xxxviii_cycle/main.cpp`.

| Frame | File | The error (g++ 13.3) |
|---|---|---|
| [Slide 48] | `slide_048_no_specifier.cpp` | `'double Vehicle::get_speed() const' is inaccessible within this context` |
| [Slide 65] | `slide_065_not_the_other_way.cpp` | `invalid conversion from 'Vehicle*' to 'Taxi*'` |
| [Slide 70] | `slide_070_private_stays_private.cpp` | `'double Vehicle::speed_' is private within this context` |
| [Slide 86] | `slide_086_private_inheritance.cpp` | `'Vehicle' is an inaccessible base of 'RoboTaxi'` |
| [Slide 98] | `slide_098_forgetting_one_override.cpp` | `cannot declare variable 'taxi' to be of abstract type 'Taxi'` |
| [Slide 104] | `slide_104_fast_robotaxi.cpp` | `cannot derive from 'final' base 'RoboTaxi' in derived type 'FastRoboTaxi'` |
| [Appendix vii] | `appendix_vii_final_method.cpp` | `virtual function 'virtual void NightTaxi::drive(const Route&) const' overriding final function` |
| [Appendix xiii] | `appendix_xiii_base_member_in_list.cpp` | `'double Vehicle::max_speed_' is private within this context`, and two more |
| [Appendix xviii] | `appendix_xviii_copying_a_vehicle.cpp` | `static assertion failed: result type must be constructible from input type`, and more |
| [Appendix xxvii] | `appendix_xxvii_overloads.cpp` | `no matching function for call to 'start_shift(const Vehicle&, Route&)'` |
| [Appendix xxxviii] | `appendix_xxxviii_cycle/` | `b.hpp:7:3: error: 'A' does not name a type` |
| [Appendix xli] | `appendix_xli_override_catches_both.cpp` | `'void RoboTaxi::drvie(const Route&) const' marked 'override', but does not override`, and one more |

A single line that does not compile, inside code that runs (such as
`base_ref.assign_driver("Ben");` on [Slide 60]), stays in `src/` as a comment,
with its error below it.
