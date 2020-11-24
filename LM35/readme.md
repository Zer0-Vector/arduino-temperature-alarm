# LM35

A library for use with an LM35 temperature sensor. It takes samples of the value, converts it to temperature, accumulates the sum of squares then returns the average when requested.

This was designed to have as little impact on runtime memory. To help with this, there are a few compile-time options listed below.

## Compile-time Options

* **LM35_SAMPLING_DELAY** is the number of milliseconds to delay after taking a sample. *Default: 25*
* **LM35_SAMPLE_SIZE** is the number of samples to collect before storing the average. *Default: 64*
* **LM35_LOAD_PIN** is the analog pin used to read the temperature sensor voltage. *Default: A0*
