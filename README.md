# Timetable Generation using Genetic Algorithm with a Population Alteration Mechanism

This code implements a genetic algorithm to generate an optimal timetable schedule. The algorithm considers various constraints and fitness criteria to create a timetable.

# Problem Description

The goal is to create a timetable schedule for a university or educational institution. The timetable should allocate subjects and labs to specific days, hours, and rooms, taking into account the following constraints:

- Each subject must be taught a certain number of times per week.
- Labs have specific hours and room requirements.
- Each faculty member has availability for teaching specific subjects.
- Certain subjects should not be scheduled back-to-back.
- Rooms have limited availability per week.
- The objective is to minimize or maximize a fitness value that represents the quality of the timetable schedule. The fitness value is calculated based on factors such as the number of subject repetitions, faculty availability, and room allocation.

# Genetic Algorithm Approach

The genetic algorithm follows these steps:

- Initialization: The algorithm starts by initializing a population of timetables with random subject and lab allocations. The population size is determined by the MAXP parameter.
- Fitness Calculation: The fitness of each timetable in the population is calculated using the fitnesscalc() function. The fitness value represents how well the timetable satisfies the given constraints and objectives.
- Parent Selection: Parents for the crossover operation are selected from the population. The number of parents selected is determined by the family parameter. The selection is based on fitness, with better-fitting individuals having a higher chance of being selected.
- Crossover: The generatenew() function performs the crossover operation between two parent timetables to generate a new timetable. Lab elements and subject elements are selected from the parents and combined to create the child timetable.
- Fitness Evaluation: The fitness of the new timetable is evaluated using the fitnesscalc() function.
- Replacement: The newly generated timetable replaces one or more parent timetables in the population. The replacement strategy is determined by the family parameter.
- Iteration: Steps 3-6 are repeated for a specified number of iterations or until a termination condition is met.
- Result: The resulting population of timetables represents the final set of generated timetables. The best timetable (based on the fitness value) can be selected as the optimal solution.

# Usage

- Ensure that the necessary input files (Faculty.txt, etc.) are present in the same directory as the code file.

- Compile and run the code using a C compiler.

- The code will generate an initial population of timetables, perform crossover operations, and replace parents based on fitness evaluations.

- The resulting population of timetables will be displayed, showing the fitness values and the allocation of subjects, labs, and rooms for each timetable.
Note

- The code can be further modified and extended to include additional constraints and improve the fitness evaluation process.
