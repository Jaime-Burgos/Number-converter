This project has been created during a weekend (aproximate time 8 hours) as a test on how fast can I make a program that converts written numbers inside phrases into digits, e.g:

    "I have ten apples" -> "I have 10 apples"

As development time has been short, there are some fucntionalities that have been left in this first approach:

* The converter does not support two-level multipliers (e.g: "One thousand and nine hundred" can't converted correctly, as it has first the multiplier "thousand" and then "hundred")

* Phrases must be harcoded, instead of passed as arguments

* Years spelled as two pairs can't be converted correctly either (e.g: "Nineteen fifty-six" should be 1956)

* Fractions are not yet supported

All those cases have been marked among the examples in the main function.

There are some other things to improve too:

* Add testing

* Use a class for the converter and separate into different files for better readability

* Imporve naming
