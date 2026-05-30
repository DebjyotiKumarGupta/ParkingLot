Problem Understanding & Requirements
1. Requirements as the contract

Functional Requirements [These define the features and behaviors of the system.]
1. Vehicle management - Support motorcycle, car, truck, and EV types. Each type has different size requirements.
2. Spot management - Motorcycle, compact, large, and electric spots. Spots exist on multiple floors.
3. Entry & exit gates - Multiple entry/exit gates. Attendant or automated ticketing at each gate.
4. Ticketing - Generate ticket on entry. Validate on exit. Each ticket records spot, time, vehicle.
5. Fee calculation - Time-based billing. Different rates by vehicle type. Payment processing.
6. Display boards - Real-time availability counts per floor, per spot type.

Non Functional Requirements [These define quality attributes, constraints, and performance expectations.]
1. Consistency - No double-booking of spots. Atomic spot assignment under concurrent load.
2. Extensibility - Design must support reservations, dynamic pricing, loyalty programs without rewrite.
3. Maintainability - Clear separation of concerns. Each class has a single, well-defined purpose.
4. Testability- Pure logic decoupled from I/O. Strategy injection for pricing, payment.

Clarifying questions to ask in the interview
1. Can a single vehicle be parked in multiple spots? (No — one spot per vehicle)
2. Can a large spot accommodate a car? (Yes, with size degradation logic)
3. Is payment always required, or can an admin waive fees? (Payment required by default)
4. How many gates do we need to support? (Assume 1–10 entry + 1–10 exit)
5. Should we handle reservations in scope? (No, but design must allow it later)
6. Is there a database? (For LLD, assume in-memory; note DB extension point)
7. Do electric vehicles need electric spots specifically? (Yes)

Real-world parking lot workflow
VEHICLE ARRIVES │ ▼ ENTRY GATE ──► Is a spot available?
No ──► Deny entry / display full
Yes ▼ SELECT SPOT ──► Assign to vehicle 
                    │ ▼ GENERATE TICKET (spot, time, vehicle, gate) (vehicle parks)  
                    │ ▼ EXIT GATE ──► Validate ticket 
                    │ ▼ CALCULATE FEE ──► Process payment 
                    │ ▼ RELEASE SPOT ──► Update display board

Scope boundary
In Scope	                            Out of Scope (extensible)
Multi-floor,                            multi-gate Reservations
4 vehicle types, 4 spot types	        Dynamic pricing
Ticket, fee, payment	                Mobile app, license plate recognition
Display boards	                        Cloud/DB integration
Admin + attendant actors	            Loyalty programs


Did they ask about concurrency?
Did they define actors?
Did they acknowledge extensibility?

Name two ways the system could fail under concurrent load. How would you mitigate them?

Double booking of a parking spot, where multiple entry gates assign the same spot simultaneously. 
I would mitigate this using atomic allocation, mutexes, or database transactions.

Inconsistent availability counts, where concurrent updates lead to incorrect display-board information. 
I would use atomic counters or synchronized inventory updates.


Object-Oriented Analysis
The goal is to identify what exists in the real world (entities).
what they know (attributes)
what they can do (responsibilities)
how they relate to each other.

How to discover classes from requirements
Underline the nouns in the requirements — these are candidate classes.
Then underline verbs — these are candidate methods or responsibilities.
Filter out primitives (strings, ints) and concepts that are attributes, not objects.

Example:
Nouns → ParkingLot, Floor, Gate, Spot, Vehicle, Ticket, Payment, DisplayBoard, Admin, Attendant, Customer
Verbs → park, release, generate (ticket), validate, calculate (fee), process (payment), update (display), assign (spot)

Composition is a strict "has-a" relationship where one object owns another, and the child object cannot exist without the parent
Association represents a structural relationship between two independent classes where one class uses or interacts with another.

ParkingLot ──has──► ParkingFloor (1 to many) [composition] 
ParkingFloor ──has──► ParkingSpot (1 to many) [composition] 
ParkingLot ──has──► EntryGate / ExitGate (1 to many) [composition] 
ParkingSpot ──is occupied by──► Vehicle (0 or 1) [association] 
EntryGate ──generates──► Ticket [dependency] 
Ticket ──references──► ParkingSpot, Vehicle [association] 
ExitGate ──triggers──► Payment [dependency] 
DisplayBoard ──observes──► ParkingFloor [observer] 
ParkingAttendant ──operates──► Gate [association]


Aggregation vs composition vs association
Composition Child cannot exist without parent (owns lifecycle) -> ParkingFloor owns ParkingSpots. Floor destroyed → spots destroyed.
Aggregation	Child can exist independently	ParkingAttendant belongs to a Gate but can be reassigned.
Association Objects collaborate but neither owns the other Vehicle is associated with a Ticket during a session.


Three important implied entities are:

SpotAllocator – responsible for finding and assigning suitable parking spots.
ParkingRate / PricingStrategy – encapsulates fee calculation rules and supports future pricing models.
PaymentProcessor – handles payment execution independently from ticketing and parking logic.

These entities are not explicitly mentioned in the noun list, but emerge when applying the 
Single Responsibility Principle and separating business concerns into dedicated objects.