# System based on time-scheduling

## Schedule: Round-robin with timer 1 interrupt

## Peripherals:
1. Timer 1
2. SPI - GLCD
3. ADC - ECG
4. FND

## Algorithm
1. Read ADC > Exact timing(1)
2. Calculate BPM > Exact timing(1)
3. Show BPM while Plotting GLCD(pulse) > Adequate timing(2)
    Update ratio:
        Plotting: slow
        FND: Fast

