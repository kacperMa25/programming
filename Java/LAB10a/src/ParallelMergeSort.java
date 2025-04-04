import java.util.Arrays;

public class ParallelMergeSort {
    public static void parallelMergeSort(int[] array, int threads) {
        if (threads <= 1) {
            mergeSort(array);
        } else if (array.length >= 2) {
            int mid = array.length / 2;
            int[] left = Arrays.copyOfRange(array, 0, mid);
            int[] right = Arrays.copyOfRange(array, mid, array.length);

            Thread leftSorter = new Thread(() -> parallelMergeSort(left, threads / 2));
            Thread rightSorter = new Thread(() -> parallelMergeSort(right, threads / 2));

            leftSorter.start();
            rightSorter.start();

            try {
                leftSorter.join();
                rightSorter.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            merge(array, left, right);
        }
    }

    private static void mergeSort(int[] array) {
        if (array.length >= 2) {
            int mid = array.length / 2;
            int[] left = Arrays.copyOfRange(array, 0, mid);
            int[] right = Arrays.copyOfRange(array, mid, array.length);

            mergeSort(left);
            mergeSort(right);

            merge(array, left, right);
        }
    }

    private static void merge(int[] result, int[] left, int[] right) {
        int i = 0, j = 0, k = 0;
        while (i < left.length && j < right.length) {
            if (left[i] <= right[j]) {
                result[k++] = left[i++];
            } else {
                result[k++] = right[j++];
            }
        }
        while (i < left.length) {
            result[k++] = left[i++];
        }
        while (j < right.length) {
            result[k++] = right[j++];
        }
    }

    public static void main(String[] args) {
        int[] array = {41, 27, 43, 23, 49, 82, 193, 123, 152, 95, 92, 429, 234, 132, 44, 53, 123};
        int threads = Runtime.getRuntime().availableProcessors(); // Liczba dostępnych wątków

        System.out.println("Tablica przed sortowaniem: " + Arrays.toString(array));
        parallelMergeSort(array, threads);
        System.out.println("Tablica po sortowaniu: " + Arrays.toString(array));
    }
}
