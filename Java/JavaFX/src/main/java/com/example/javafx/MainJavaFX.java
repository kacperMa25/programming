package com.example.javafx;

import javafx.application.Application;
import javafx.event.*;
import javafx.stage.Stage;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.*;


public class MainJavaFX extends Application {
    private Label label;

    @Override
    public void start(Stage primaryStage) {

        label = new Label("Empty");
        Button btn = new Button();
        btn.setText("0");
        btn.setOnAction(new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent event) {
                Button btn2 = (Button) event.getSource();
                btn2.setText(String.valueOf(Integer.parseInt(btn.getText()) + 1));
                label.setText(btn2.getText());
            }
        });
//StackPane root = new StackPane();
        GridPane root = new GridPane();
        root.hgapProperty().set(10);
        root.vgapProperty().set(10);
        root.add(btn, 0, 0);
        root.add(label, 2, 3);
        Scene scene = new Scene(root, 300, 250);
        primaryStage.setTitle("Hello World!");
        primaryStage.setScene(scene);
        primaryStage.show();
    }



    public static void main(String[] args) {
        launch(args);
    }
}
