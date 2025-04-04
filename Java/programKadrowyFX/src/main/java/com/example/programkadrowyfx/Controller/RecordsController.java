package com.example.programkadrowyfx.Controller;

import com.example.programkadrowyfx.Model.Pracownik;
import com.example.programkadrowyfx.View.RecordsViewFX;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.stage.FileChooser;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class RecordsController {

    private final RecordsViewFX view;
    private final ObservableList<Pracownik> pracownikList;

    public RecordsController(RecordsViewFX view) {
        this.view = view;
        this.pracownikList = FXCollections.observableArrayList();

        setupEventHandlers();
    }

    private void setupEventHandlers() {
        view.setOnAddPracownik(event -> {
            Pracownik newPracownik = view.showAddPracownikDialog();
            if (newPracownik != null) {
                addPracownik(newPracownik);
            }
        });

        view.setOnDeletePracownik(event -> {
            Pracownik selectedPracownik = view.getSelectedPracownik();
            if (selectedPracownik != null) {
                deletePracownik(selectedPracownik);
            } else {
                view.showError("No selection", "Please select a record to delete.");
            }
        });

        view.setOnViewDetails(event -> {
            Pracownik selectedPracownik = view.getSelectedPracownik();
            if (selectedPracownik != null) {
                view.showPracownikDetails(selectedPracownik);
            } else {
                view.showError("No selection", "Please select a record to view details.");
            }
        });
    }

    public void addPracownik(Pracownik pracownik) {
        pracownikList.add(pracownik);
        view.updateTable(pracownikList);
    }

    public void deletePracownik(Pracownik pracownik) {
        pracownikList.remove(pracownik);
        view.updateTable(pracownikList);
    }

    public List<Pracownik> getPracownikList() {
        return new ArrayList<>(pracownikList);
    }
}
