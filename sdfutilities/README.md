# Basic SDF to Ascii

It's easier to simlink the exe into wherever your data is. Create a CSV with:
```sh 
dosbox -c "mount c $(pwd)" -c "c:" -c "sdftoasc.exe dark.dat dark.csv /O /X /Y:D /B:\",\" /T:R,I " -c "exit"
```

This puts out the frequency and the real and imaginary components of the power density (so there's not actually any imaginary component).
