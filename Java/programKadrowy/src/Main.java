import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.layout.BorderPane;
import javafx.stage.Stage;

import

public class RecordsApp extends Application {
    @Override
    public void start(Stage primaryStage) {
        RecordsViewFX recordsView = new RecordsViewFX();
        RecordsController controller = new RecordsController(recordsView);

        BorderPane root = recordsView.getMainLayout();
        primaryStage.setScene(new Scene(root, 800, 600));
        primaryStage.setTitle("Records Management");
        primaryStage.show();
    }

    public static void main(String[] args) {
        launch(args);
    }
}
