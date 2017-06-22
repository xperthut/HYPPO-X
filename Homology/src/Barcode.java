
/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: Barcode.java
 Objective: Barcode image for persistent homology
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              07/11/2016      Initial version
 
 **************************************************************************************************/

package barcode;

import edu.stanford.math.plex4.homology.barcodes.BarcodeCollection;
import edu.stanford.math.plex4.homology.chain_basis.Simplex;
import edu.stanford.math.plex4.homology.interfaces.AbstractPersistenceAlgorithm;
import edu.stanford.math.plex4.streams.impl.ExplicitSimplexStream;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

public class Barcode {

    public void GetStreamForData(ExplicitSimplexStream stream) {
        /*
            Add the simplex code generated from our main project writen in C++
        */
    }

    public static void main(String[] args) {
        Barcode barcode_ = new Barcode();
        int dimension = 4;

        ExplicitSimplexStream stream = edu.stanford.math.plex4.api.Plex4.createExplicitSimplexStream();

        barcode_.GetStreamForData(stream);
        stream.finalizeStream();

        int slices = stream.getSize();
        System.out.println("Total simplices=" + slices);

        AbstractPersistenceAlgorithm<Simplex> persistent
                = edu.stanford.math.plex4.api.Plex4.getDefaultSimplicialAlgorithm(dimension);
        BarcodeCollection<Double> intervals_index = persistent.computeIntervals(stream);

        try {
            edu.stanford.math.plex4.api.Plex4.createBarcodePlot(intervals_index, "Barcode", 50);
        } catch (IOException ex) {
            Logger.getLogger(Barcode.class.getName()).log(Level.SEVERE, null, ex);
        }

    }

}
