import java.util.Random;

abstract class Matrix {
    protected final int rows;
    protected final int cols;
    protected double[][] data;

    public Matrix(int rows, int cols) {
        this.rows = rows;
        this.cols = cols;
        this.data = new double[rows][cols];
    }

    public Matrix(double[][] data) {
        this.rows = data.length;
        this.cols = data[0].length;
        this.data = new double[rows][cols];
        for (int i = 0; i < rows; i++) {
            System.arraycopy(data[i], 0, this.data[i], 0, cols);
        }
    }

    public int getRows() {
        return rows;
    }

    public int getCols() {
        return cols;
    }

    public void print() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                System.out.print(data[i][j] + " ");
            }
            System.out.println();
        }
    }

    protected void checkSameDimension(Matrix other) {
        if (this.rows != other.rows || this.cols != other.cols) {
            throw new IllegalArgumentException("Matrices dimensions do not match.");
        }
    }
}

class MutableMatrix extends Matrix {

    public MutableMatrix(int rows, int cols) {
        super(rows, cols);
    }

    public MutableMatrix(double[][] data) {
        super(data);
    }

    public MutableMatrix(InmutableMatrix immutableMatrix) {
        super(immutableMatrix.data);
    }

    public MutableMatrix add(MutableMatrix other) {
        checkSameDimension(other);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                this.data[i][j] += other.data[i][j];
            }
        }
        return this;
    }

    public MutableMatrix subtract(MutableMatrix other) {
        checkSameDimension(other);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                this.data[i][j] -= other.data[i][j];
            }
        }
        return this;
    }

    public MutableMatrix multiply(double scalar) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                this.data[i][j] *= scalar;
            }
        }
        return this;
    }

    public MutableMatrix transpose() {
        double[][] transposed = new double[cols][rows];
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                transposed[j][i] = this.data[i][j];
            }
        }
        this.data = transposed;
        return this;
    }

    public InmutableMatrix toInmutableMatrix() {
        return new InmutableMatrix(this);
    }
}

final class InmutableMatrix extends Matrix {

    public InmutableMatrix(int rows, int cols) {
        super(rows, cols);
    }

    public InmutableMatrix(double[][] data) {
        super(data);
    }

    public InmutableMatrix(MutableMatrix mutableMatrix) {
        super(mutableMatrix.data);
    }

    public InmutableMatrix add(InmutableMatrix other) {
        checkSameDimension(other);
        double[][] result = new double[rows][cols];
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result[i][j] = this.data[i][j] + other.data[i][j];
            }
        }
        return new InmutableMatrix(result);
    }

    public InmutableMatrix subtract(InmutableMatrix other) {
        checkSameDimension(other);
        double[][] result = new double[rows][cols];
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result[i][j] = this.data[i][j] - other.data[i][j];
            }
        }
        return new InmutableMatrix(result);
    }

    public InmutableMatrix multiply(double scalar) {
        double[][] result = new double[rows][cols];
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result[i][j] = this.data[i][j] * scalar;
            }
        }
        return new InmutableMatrix(result);
    }

    public InmutableMatrix transpose() {
        double[][] transposed = new double[cols][rows];
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                transposed[j][i] = this.data[i][j];
            }
        }
        return new InmutableMatrix(transposed);
    }

    public MutableMatrix toMutableMatrix() {
        return new MutableMatrix(this);
    }
}

public class MatrixTest {

    public static void main(String[] args) {
        // 增大运算量，使用 1000 x 1000 的矩阵
        int size = 1000;
        double[][] data1 = generateRandomMatrix(size, size);
        double[][] data2 = generateRandomMatrix(size, size);

        MutableMatrix m1 = new MutableMatrix(data1);
        MutableMatrix m2 = new MutableMatrix(data2);
        InmutableMatrix im1 = new InmutableMatrix(data1);
        InmutableMatrix im2 = new InmutableMatrix(data2);

        // 测试 MutableMatrix
        long startMutable = System.nanoTime();
        m1.add(m2).multiply(1.5).subtract(m2).transpose();
        long endMutable = System.nanoTime();
        System.out.println("MutableMatrix 运算时间: " + (endMutable - startMutable) / 1_000_000.0 + " 毫秒");

        // 测试 InmutableMatrix
        long startInmutable = System.nanoTime();
        InmutableMatrix result = im1.add(im2).multiply(1.5).subtract(im2).transpose();
        long endInmutable = System.nanoTime();
        System.out.println("InmutableMatrix 运算时间: " + (endInmutable - startInmutable) / 1_000_000.0 + " 毫秒");
    }

    // 随机矩阵生成器
    private static double[][] generateRandomMatrix(int rows, int cols) {
        Random rand = new Random();
        double[][] matrix = new double[rows][cols];
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                matrix[i][j] = rand.nextDouble() * 100;
            }
        }
        return matrix;
    }
}
