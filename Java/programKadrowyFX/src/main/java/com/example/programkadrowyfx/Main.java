package com.example.programkadrowyfx;

import com.example.programkadrowyfx.Controller.RecordsController;
import com.example.programkadrowyfx.View.RecordsViewFX;
import javafx.application.Application;
import javafx.stage.Stage;

public class Main extends Application {
    @Override
    public void start(Stage primaryStage) {
        RecordsViewFX view = new RecordsViewFX(primaryStage);
        RecordsController controller = new RecordsController(view);


        view.show();
    }

    public static void main(String[] args) {
        launch(args);
    }
}
