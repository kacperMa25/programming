import java.util.Scanner;
import java.io.*;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;
import java.util.zip.ZipInputStream;

public class Compressor {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        while (true){
            System.out.println("What whould you like to do?");
            System.out.println("1. Compress");
            System.out.println("2. Decompress");
            System.out.println("3. Exit");
            System.out.print("Enter your choice: ");

            int choice = scanner.nextInt();
            scanner.nextLine();

            switch (choice){
                case 1:
                    compress(scanner);
                    break;
                case 2:
                    decompress(scanner);
                    break;
                case 3:
                    System.out.println("Goodbye!");
                    scanner.close();
                    return;
                default:
                    System.out.println("Invalid choice\n");
            }
        }
    }

    private static void compress(Scanner scanner){
        System.out.println("Enter text you want to compress\n");
        StringBuilder text = new StringBuilder();
        while (true){
            String line = scanner.nextLine();
            if (line.equals("END")){
                break;
            }
            text.append(line).append("\n");
        }

        System.out.print("Enter file name: ");
        String filename = scanner.nextLine();

        try(FileOutputStream fos = new FileOutputStream(filename);
        ZipOutputStream zos = new ZipOutputStream(fos)){
            ZipEntry ze = new ZipEntry("compressed_"+filename);
            zos.putNextEntry(ze);
            zos.write(text.toString().getBytes());
            zos.closeEntry();
            System.out.println("Compressed "+filename+" successfully");
        } catch (IOException e){
            System.out.println("Something went wrong ZIPing file: " + e.getMessage());
        }
    }

    private static void decompress(Scanner scanner){
        System.out.println("Enter file name: ");
        String filename = scanner.nextLine();

        try (FileInputStream fis = new FileInputStream(filename);
        ZipInputStream zis = new ZipInputStream(fis)){
            ZipEntry ze = zis.getNextEntry();
            if (ze == null){
                System.out.println("File not found");
                return;
            }
            System.out.println("Decompressed text: ");
            BufferedReader br = new BufferedReader(new InputStreamReader(zis));
            String line;
            while ((line = br.readLine()) != null){
                System.out.println(line);
            }
            zis.closeEntry();
        } catch (IOException e){
            System.out.println("Something went wrong unZIPing file: " + e.getMessage());
        }
    }
}