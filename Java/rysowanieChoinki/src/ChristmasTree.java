import javax.swing.*;
import java.awt.*;

public class ChristmasTree extends JPanel {
    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);

        setBackground(Color.PINK);

        g.fillRect(0, 330, getWidth(), getHeight());

        g.setColor(new Color(139, 69, 19)); // Brown color
        g.fillRect(180, 300, 40, 100);

        g.setColor(Color.GREEN);
        int[] xPoints1 = {200, 120, 280};
        int[] yPoints1 = {100, 200, 200};
        g.fillPolygon(xPoints1, yPoints1, 3);

        int[] xPoints2 = {200, 110, 290};
        int[] yPoints2 = {150, 250, 250};
        g.fillPolygon(xPoints2, yPoints2, 3);

        int[] xPoints3 = {200, 100, 300};
        int[] yPoints3 = {200, 300, 300};
        g.fillPolygon(xPoints3, yPoints3, 3);

        int[] xPoints4 = {200, 90, 310};
        int[] yPoints4 = {250, 350, 350};
        g.fillPolygon(xPoints4, yPoints4, 3);

        g.setColor(Color.YELLOW);
        g.fillOval(190, 90, 20, 20);

        g.setColor(Color.RED);
        g.fillOval(190, 120, 20, 20);
        g.fillOval(150, 170, 20, 20);
        g.fillOval(230, 170, 20, 20);

        g.setColor(Color.BLUE);
        g.fillOval(170, 240, 20, 20);
        g.fillOval(210, 240, 20, 20);
        g.fillOval(143, 300, 20, 20);
        g.fillOval(235, 300, 20, 20);
        g.setColor(Color.MAGENTA);

        g.fillOval(190, 312, 20, 20);
        g.fillOval(190, 205, 20, 20);

        g.setColor(Color.RED);
        g.fillRect(100, 400, 40, 40);
        g.fillRect(270, 380, 50, 40);

        g.setColor(Color.BLUE);
        g.fillRect(90, 450, 70, 40);
        g.fillRect(225, 390, 40, 90);
    }

    public static void main(String[] args) {
        JFrame frame = new JFrame("Christmas Tree");
        ChristmasTree tree = new ChristmasTree();
        frame.add(tree);
        frame.setSize(400, 500);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setVisible(true);
    }
}
