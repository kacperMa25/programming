package org.example;

import Model.Dyrektor;
import Model.Handlowiec;
import org.junit.Test;

import Controller.RecordsController;
import View.RecordsView;

import java.io.IOException;

import static org.junit.Assert.*;

public class RecordTests {
    @Test
    public void testAddHandlowiecToEmptyContainer() {
        Handlowiec handlowiec = new Handlowiec("54021234629", "Jan", "Kowalski", "Handlowiec", 5000, "001", 10, 1000);
        RecordsView recordsView = new RecordsView();
        RecordsController recordsController = new RecordsController(recordsView);

        assertTrue(recordsController.addWorker(handlowiec));
    }

    @Test
    public void testAddDyrektorToEmptyContainer() {
        Dyrektor dyrektor = new Dyrektor("54021234629", "Kacper", "Małecki", "Dyrektor", 123, "123", 123, "123", 123);
        RecordsView recordsView = new RecordsView();
        RecordsController recordsController = new RecordsController(recordsView);

        assertTrue(recordsController.addWorker(dyrektor));
    }

    @Test
    public void testAddHandlowiecToNonEmptyContainer() {
        RecordsView recordsView = new RecordsView();
        RecordsController recordsController = new RecordsController(recordsView);
        recordsController.addWorker(new Dyrektor("50050626141", "Anna", "Nowak", "Dyrektor", 10000, "002", 2000, "ABC123", 5000));

        Handlowiec handlowiec = new Handlowiec("65102122555", "Jan", "Kowalski", "Handlowiec", 5000, "001", 10, 1000);
        assertTrue(recordsController.addWorker(handlowiec));
    }

    @Test
    public void testAddDyrektorToNonEmptyContainer() {
        RecordsView recordsView = new RecordsView();
        RecordsController recordsController = new RecordsController(recordsView);
        recordsController.addWorker(new Handlowiec("65102122555", "Jan", "Kowalski", "Handlowiec", 5000, "001", 10, 1000));
        Dyrektor dyrektor = new Dyrektor("73120515651", "Anna", "Nowak", "Dyrektor", 10000, "002", 2000, "ABC123", 5000);
        assertTrue(recordsController.addWorker(dyrektor));
    }

    @Test
    public void testAddRandom10(){
        String[] PESELs = {"81050427494",
        "09221454588",
        "50050626141",
        "55041949168",
        "82121278188",
        "65102122555",
        "00280929422",
        "54021234629",
        "73120515651",
        "77051119135"};

        RecordsView recordsView = new RecordsView();
        RecordsController recordsController = new RecordsController(recordsView);

        for(int i = 0; i < 10; ++i){
            int random = (int) (Math.random() * 2);
            if (random == 0){
                assertTrue(recordsController.addWorker(new Handlowiec(PESELs[i], "Jan", "Kowalski", "Handlowiec", 5000, "001", 10, 1000)));
            } else {
                assertTrue(recordsController.addWorker(new Dyrektor(PESELs[i], "Anna", "Nowak", "Dyrektor", 10000, "002", 2000, "ABC123", 5000)));
            }
        }
    }

    @Test
    public void testRemoveHandlowiecFromNonEmptyContainer() {
        RecordsView recordsView = new RecordsView();
        RecordsController recordsController = new RecordsController(recordsView);
        recordsController.addWorker(new Dyrektor("50050626141", "Anna", "Nowak", "Dyrektor", 10000, "002", 2000, "ABC123", 5000));
        Handlowiec handlowiec = new Handlowiec("65102122555", "Jan", "Kowalski", "Handlowiec", 5000, "001", 10, 1000);
        recordsController.addWorker(handlowiec);
        assertTrue(recordsController.removeWorker(handlowiec.getPesel()));
    }

    @Test
    public void testRemoveDyrektorToNonEmptyContainer() {
        RecordsView recordsView = new RecordsView();
        RecordsController recordsController = new RecordsController(recordsView);
        recordsController.addWorker(new Handlowiec("65102122555", "Jan", "Kowalski", "Handlowiec", 5000, "001", 10, 1000));
        Dyrektor dyrektor = new Dyrektor("73120515651", "Anna", "Nowak", "Dyrektor", 10000, "002", 2000, "ABC123", 5000);
        recordsController.addWorker(dyrektor);
        assertTrue(recordsController.removeWorker(dyrektor.getPesel()));
    }

    @Test
    public void testSerialization(){
        RecordsView recordsView = new RecordsView();
        RecordsController recordsController = new RecordsController(recordsView);

        recordsController.addWorker(new Handlowiec("12345678901", "Jan", "Kowalski", "Handlowiec", 5000, "001", 10, 1000));

        try {
            recordsController.compressZip("workers.zip");
        } catch (IOException e) {
            fail(e.getMessage());
        }
    }

    @Test
    public void testDeserialization() {
        RecordsView recordsView = new RecordsView();
        RecordsController recordsController = new RecordsController(recordsView);

        assertTrue(recordsController.decompress("workers.zip"));

    }

    @Test
    public void testInvalidDeserialization() {
        RecordsView recordsView = new RecordsView();
        RecordsController recordsController = new RecordsController(recordsView);

        assertFalse(recordsController.decompress("corupted.z"));
    }


}
