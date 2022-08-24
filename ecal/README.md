# LDMX ECal Detector Model
This is just a dump for intermediate files while developing the LDMX ECal
detector model.

The [stl](stl) directory contains some of the more complicated shapes from
the ECal engineering design[^1] which are converted to GDML using [an
STL to GDML converter](https://github.com/tihonav/cad-to-geant4-converter)
I've found on GitHub.

```
python3 stl_to_gdml/stl_gdml.py complicated_shapes stl/*
```

Produces a pile of GDML tesselated solids to sift through
(or copy to the detector description).



[^1]: Thank you Susanne Kyre from UCSB.

