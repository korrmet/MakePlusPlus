# Preface

Make++ is an all-in-one software project system. It's a console utility
that may run in every system. Aim of this project is to ease the
product maintainance.

Main principles of the Make++:

1) Every action is clear and explicit.
2) One file is all you need to control your project.
3) No weird synthax consisted of single-character commands.
4) You can't overcomplicate your project. It always be plain.
5) There is no any programming. Just specification what you need.

The main pattern of Make++ is processing items. To process anything you
should answer next questions:
1) What to process? What is input?
2) What is the goal? What is output?
3) How it should be processed? What program need to process this?
4) When it should be processed? What is the pre-requisites?

Files of project have a '.mpp' extension. Program analyzes that file,
creates tree with all possible jobs, analyzes the project sources state,
marks jobs to be done and try to run it.

Make++ file divided by sections respectively the functionality. The synthax
is simple: you declare fields and their values.

Make++ avoids to shit in your project like a CMake. But it still needs just
a two files:
1) *.mpp.temp - storage for temporary information, like a sources stata
2) *.mpp.conf - current configuration of the project you defined

Make++ can work in a monitoring mode. It checks the project with specified
time interval and perform all of the available jobs for changed files.
GUI mode have a dashboard-like interface, text mode shows you all of the
errors only. Monitoring mode checks requirements, tests, and build.

# Functionality

Make++ helps you in next cases:

1) Project configuration.
2) Requirements.
3) Statical analysis.
4) Testing.
5) Building.
6) Deployment.

## Project configuration

Here all of the project variables should be defined. Also there is a
configuration menu declaration, like KConfig. You can use these variables
everywhere.

You can configure these parameters in GUI-editor or using command-line
arguments.

Every menu item should have a name, variable name, and optionally list of
values available. Value is always just a string.

## Requirements

Here you enlist the requirements. Make++ can generate reports about
requirements fulfilment. It checks in the source files special marks
with the name of requirement and if it exists it mark it as fulfilled.

## Statical analysis

This section identifies tools to checking the source files.

## Testing

Test is just an executable that returns zero if test succeed and non-zero
if fails. Nothing more. Do anything what you want inside that file.

## Building

Build consists of build artifacts, their dependencies and rules of
processing.

## Deployment

Here you can define what to do with processing results, where to place
them.
