# Theboys

This repository contains the final project for Programming 1 "The Boys" (2025/2 version) at the Federal University of Paraná (UFPR),
under the supervision of Professor Giovanni Venâncio.The project was inspired by the TV series "The Boys" and involves a simulation-based scenario with heroes, bases, and missions.

## Scenario Concept
The simulation models a world inhabited by heroes, each defined by a unique set of skills (rated 0-10) and an evolving experience level. These heroes dynamically interact with a network of bases and respond to emerging missions throughout the simulation.

## Core Mechanics
Heroes and Bases:
Hero units regularly travel between different bases. Each base has a finite capacity, necessitating a queuing system for entry.
Upon arrival at a full base, a hero may either enter the queue or opt to travel to a different base.
Base logistics, including managing the queue and processing hero arrivals and departures, are handled by designated "gatekeeper" processes.

## Missions:
In this project, missions function as dynamic events designed to test the heroes. Essentially, a mission appears at a random location and requires a specific set of skills to be resolved.

The system first identifies the base closest to the mission's location. It then evaluates the team of heroes currently at that base to see if their combined skills meet all the mission's requirements.

Mission Outcomes:
Success: If the team at the closest base is qualified, they are selected for the mission, and all participating members gain experience points.
Failure (Impossible Mission): If the closest team is not qualified (or if no team at any base is), the mission is flagged as "impossible" and postponed for 24 hours.
Contingency (Compound V): In specific, rare situations (at set time intervals) where no team can complete the mission, a last-resort mechanic is available. The most experienced hero at the closest base can use "Compound V." This guarantees the mission's success but comes at a tragic cost: the hero dies immediately after its use.

## Simulation duration
The World runs for the duration of a one in-game year (equivalent to 525,600 minutes).
Throughout this year, the sistem logs all significant events.

## Technical Specifications
This project must be developed using the C programming language. A key requirement is the use of highly efficient data structures to manage all entities and events. Key areas of focus include:

Data Structure Efficiency: Selecting optimal structures for managing hero data, base queues, mission criteria, and event handling.
Algorithmic Design: Developing effective algorithms for base logistics and for the hero-team selection process.
Code Modularity: Structuring the code in a modular fashion to facilitate testing, maintenance, and future expansion.
Performance: Optimizing the simulation to ensure it can run efficiently, even with a large volume of entities and events.

## Expected Output
Upon completion of the simulation run, the program is expected to produce:

  A detailed event log itemizing all actions and occurrences.
  Statistical summaries, including the total number of missions completed by each hero.
  The final experience point totals for every hero. 
  Metrics on base utilization and other data reflecting the system's overall dynamics.
