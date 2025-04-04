package com.example.programkadrowyfx.View;

import com.example.programkadrowyfx.Controller.GzipCompression;
import com.example.programkadrowyfx.Controller.ZipCompression;
import com.example.programkadrowyfx.Model.Dyrektor;
import com.example.programkadrowyfx.Model.Handlowiec;
import com.example.programkadrowyfx.Model.Pracownik;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.stage.FileChooser;
import javafx.stage.Stage;

import java.io.File;
import java.io.IOException;
import java.util.Optional;

public class RecordsViewFX {

    private final Stage primaryStage;
    private final TableView<Pracownik> pracownikTable;
    private final Button addButton;
    private final Button deleteButton;
    private final Button viewDetailsButton;
    private final Button zipButton;
    private final Button unzipButton;
    private final Button gzipButton;
    private final Button ungzipButton;

    public RecordsViewFX(Stage primaryStage) {
        this.primaryStage = primaryStage;
        this.pracownikTable = new TableView<>();
        this.addButton = new Button("Add");
        this.deleteButton = new Button("Delete");
        this.viewDetailsButton = new Button("View Details");
        this.zipButton = new Button("Compress to ZIP");
        this.unzipButton = new Button("Decompress ZIP");
        this.gzipButton = new Button("Commpress to GZIP");
        this.ungzipButton = new Button("Decompress GZIP");


        initializeUI();
    }

    private void initializeUI() {
        primaryStage.setTitle("Employee Records");

        zipButton.setOnAction(event -> {
            FileChooser fileChooser = new FileChooser();
            File file = fileChooser.showOpenDialog(primaryStage);
            if (file != null) {
                try {
                    ZipCompression.compressToZip(file.getAbsolutePath(), "output.zip");
                } catch (IOException e) {
                    showError("Error", "An error occurred during compression.");
                }
            }
        });

        unzipButton.setOnAction(event -> {
            FileChooser fileChooser = new FileChooser();
            File file = fileChooser.showOpenDialog(primaryStage);
            if (file != null) {
                try {
                    ZipCompression.decompressZip(file.getAbsolutePath(), "output");
                } catch (IOException e) {
                    showError("Error", "An error occurred during decompression.");
                }
            }
        });

        gzipButton.setOnAction(event -> {
            FileChooser fileChooser = new FileChooser();
            File file = fileChooser.showOpenDialog(primaryStage);
            if (file != null) {
                try {
                    GzipCompression.compressToGzip(file.getAbsolutePath(), "output.gz");
                } catch (IOException e) {
                    showError("Error", "An error occurred during compression.");
                }
            }
        });

        ungzipButton.setOnAction(event -> {
            FileChooser fileChooser = new FileChooser();
            File file = fileChooser.showOpenDialog(primaryStage);
            if (file != null) {
                try {
                    GzipCompression.decompressGzip(file.getAbsolutePath(), "output_file");
                } catch (IOException e) {
                    showError("Error", "An error occurred during decompression.");
                }
            }
        });

        // Table setup
        TableColumn<Pracownik, String> peselCol = new TableColumn<>("PESEL");
        peselCol.setCellValueFactory(cellData -> new javafx.beans.property.SimpleStringProperty(cellData.getValue().getPesel()));

        TableColumn<Pracownik, String> imieCol = new TableColumn<>("First Name");
        imieCol.setCellValueFactory(cellData -> new javafx.beans.property.SimpleStringProperty(cellData.getValue().getImie()));

        TableColumn<Pracownik, String> nazwiskoCol = new TableColumn<>("Last Name");
        nazwiskoCol.setCellValueFactory(cellData -> new javafx.beans.property.SimpleStringProperty(cellData.getValue().getNazwisko()));

        TableColumn<Pracownik, String> stanowiskoCol = new TableColumn<>("Position");
        stanowiskoCol.setCellValueFactory(cellData -> new javafx.beans.property.SimpleStringProperty(cellData.getValue().getStowisko()));

        TableColumn<Pracownik, String> telefonCol = new TableColumn<>("Phone Number"); // Nowa kolumna na telefon
        telefonCol.setCellValueFactory(cellData -> new javafx.beans.property.SimpleStringProperty(cellData.getValue().getNumerSluzbowy()));

        TableColumn<Pracownik, Number> wynagrodzenieCol = new TableColumn<>("Salary");
        wynagrodzenieCol.setCellValueFactory(cellData -> new javafx.beans.property.SimpleIntegerProperty(cellData.getValue().getWynagrodzenie()));

        TableColumn<Pracownik, String> dodatkoweCol = new TableColumn<>("Additional Info");
        dodatkoweCol.setCellValueFactory(cellData -> new javafx.beans.property.SimpleStringProperty(cellData.getValue().getAdditionalInfo()));

        pracownikTable.getColumns().addAll(peselCol, imieCol, nazwiskoCol, stanowiskoCol, wynagrodzenieCol, telefonCol, dodatkoweCol);

        HBox buttonBox = new HBox(10, addButton, deleteButton, viewDetailsButton, zipButton, unzipButton, gzipButton, ungzipButton);
        buttonBox.setPadding(new Insets(10));

        BorderPane root = new BorderPane();
        root.setCenter(pracownikTable);
        root.setBottom(buttonBox);

        Scene scene = new Scene(root, 800, 600);
        primaryStage.setScene(scene);
    }

    public void show() {
        primaryStage.show();
    }

    public Pracownik getSelectedPracownik() {
        return pracownikTable.getSelectionModel().getSelectedItem();
    }

    public void updateTable(ObservableList<Pracownik> pracownikList) {
        pracownikTable.setItems(pracownikList);
    }

    public void showError(String title, String message) {
        Alert alert = new Alert(Alert.AlertType.ERROR);
        alert.setTitle(title);
        alert.setHeaderText(null);
        alert.setContentText(message);
        alert.showAndWait();
    }

    public Pracownik showAddPracownikDialog() {
        Dialog<Pracownik> dialog = new Dialog<>();
        dialog.setTitle("Add Employee");

        ComboBox<String> stanowiskoComboBox = new ComboBox<>();
        stanowiskoComboBox.getItems().addAll("Handlowiec", "Dyrektor");

        TextField peselField = new TextField();
        TextField imieField = new TextField();
        TextField nazwiskoField = new TextField();
        TextField numerTelefonuField = new TextField(); // Zmiana na numer telefonu
        TextField wynagrodzenieField = new TextField();

        TextField prowizjaField = new TextField();
        TextField limitField = new TextField();
        TextField dodatekField = new TextField();
        TextField kartaField = new TextField();

        VBox fieldsBox = new VBox(10,
                createLabeledField("PESEL", peselField),
                createLabeledField("First Name", imieField),
                createLabeledField("Last Name", nazwiskoField),
                createLabeledField("Phone Number", numerTelefonuField),
                createLabeledField("Salary", wynagrodzenieField),
                createLabeledField("Position", stanowiskoComboBox)
        );

        stanowiskoComboBox.setOnAction(event -> {
            String selectedStanowisko = stanowiskoComboBox.getValue();
            fieldsBox.getChildren().clear();

            fieldsBox.getChildren().addAll(
                    createLabeledField("PESEL", peselField),
                    createLabeledField("First Name", imieField),
                    createLabeledField("Last Name", nazwiskoField),
                    createLabeledField("Phone Number", numerTelefonuField),
                    createLabeledField("Salary", wynagrodzenieField),
                    createLabeledField("Position", stanowiskoComboBox)
            );

            if ("Handlowiec".equals(selectedStanowisko)) {
                fieldsBox.getChildren().addAll(
                        createLabeledField("Commission", prowizjaField),
                        createLabeledField("Limit", limitField)
                );
            } else if ("Dyrektor".equals(selectedStanowisko)) {
                fieldsBox.getChildren().addAll(
                        createLabeledField("Bonus", dodatekField),
                        createLabeledField("Card Number", kartaField),
                        createLabeledField("Limit", limitField)
                );
            }
        });

        stanowiskoComboBox.getSelectionModel().select(0);
        stanowiskoComboBox.fireEvent(new ActionEvent());

        dialog.getDialogPane().setContent(fieldsBox);
        dialog.getDialogPane().getButtonTypes().addAll(ButtonType.OK, ButtonType.CANCEL);

        dialog.setResultConverter(buttonType -> {
            if (buttonType == ButtonType.OK) {
                try {
                    String pesel = peselField.getText();
                    String imie = imieField.getText();
                    String nazwisko = nazwiskoField.getText();
                    String numerTelefonu = numerTelefonuField.getText();
                    int wynagrodzenie = Integer.parseInt(wynagrodzenieField.getText());
                    String stanowisko = stanowiskoComboBox.getValue();

                    if (Pracownik.isPeselCorrect(pesel) == 0){
                        showError("PESEL incorrect", "Please enter valid pesel");
                    }
                    else if ("Handlowiec".equals(stanowisko)) {
                        int prowizja = Integer.parseInt(prowizjaField.getText());
                        int limit = Integer.parseInt(limitField.getText());
                        return new Handlowiec(pesel, imie, nazwisko, stanowisko, wynagrodzenie, numerTelefonu, prowizja, limit);
                    } else if ("Dyrektor".equals(stanowisko)) {
                        int dodatek = Integer.parseInt(dodatekField.getText());
                        String karta = kartaField.getText();
                        int limit = Integer.parseInt(limitField.getText());
                        return new Dyrektor(pesel, imie, nazwisko, stanowisko, wynagrodzenie, numerTelefonu, dodatek, karta, limit);
                    }
                } catch (NumberFormatException e) {
                    showError("Invalid Input", "Please enter valid numerical values for salary and other fields.");
                }
            }
            return null;
        });

        Optional<Pracownik> result = dialog.showAndWait();
        return result.orElse(null);
    }

    public void showPracownikDetails(Pracownik pracownik) {
        Alert alert = new Alert(Alert.AlertType.INFORMATION);
        alert.setTitle("Employee Details");
        alert.setHeaderText(null);
        alert.setContentText(pracownik.toString());
        alert.showAndWait();
    }

    private HBox createLabeledField(String label, TextField field) {
        Label lbl = new Label(label + ":");
        lbl.setPrefWidth(100);
        return new HBox(10, lbl, field);
    }

    private HBox createLabeledField(String label, ComboBox<String> comboBox) {
        Label lbl = new Label(label + ":");
        lbl.setPrefWidth(100);
        return new HBox(10, lbl, comboBox);
    }

    public void setOnAddPracownik(EventHandler<ActionEvent> handler) {
        addButton.setOnAction(handler);
    }

    public void setOnDeletePracownik(EventHandler<ActionEvent> handler) {
        deleteButton.setOnAction(handler);
    }

    public void setOnViewDetails(EventHandler<ActionEvent> handler) {
        viewDetailsButton.setOnAction(handler);
    }

    public void setOnZip(EventHandler<ActionEvent> handler) {
        zipButton.setOnAction(handler);
    }
    public void setOnUnzip(EventHandler<ActionEvent> handler) {
        unzipButton.setOnAction(handler);
    }

    public void setOnGzip(EventHandler<ActionEvent> handler) {
        gzipButton.setOnAction(handler);
    }

    public void setOnUngzip(EventHandler<ActionEvent> handler) {
        ungzipButton.setOnAction(handler);
    }
}
