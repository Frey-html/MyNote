import java.util.Random;
import java.util.Scanner;

class SudokuGenerator {
    private static final int SIZE = 9; // 9x9 数独
    private int[][] board;

    public SudokuGenerator() {
        board = new int[SIZE][SIZE];
    }

    // 生成完整的数独
    public int[][] generateFullBoard() {
        fillBoard(0, 0);
        return board;
    }

    // 填充整个数独板
    private boolean fillBoard(int row, int col) {
        if (row == SIZE) {
            return true; // 成功填充所有行
        }

        int nextRow = col == SIZE - 1 ? row + 1 : row;
        int nextCol = col == SIZE - 1 ? 0 : col + 1;

        Random rand = new Random();
        int[] numbers = rand.ints(1, SIZE + 1).distinct().limit(SIZE).toArray();

        for (int num : numbers) {
            if (isValidPlacement(row, col, num)) {
                board[row][col] = num;
                if (fillBoard(nextRow, nextCol)) {
                    return true;
                }
                board[row][col] = 0; // 回溯
            }
        }
        return false;
    }

    // 验证某个数字是否可以放在给定位置
    private boolean isValidPlacement(int row, int col, int num) {
        // 检查行和列
        for (int i = 0; i < SIZE; i++) {
            if (board[row][i] == num || board[i][col] == num) {
                return false;
            }
        }

        // 检查 3x3 小宫格
        int boxRowStart = row / 3 * 3;
        int boxColStart = col / 3 * 3;
        for (int i = boxRowStart; i < boxRowStart + 3; i++) {
            for (int j = boxColStart; j < boxColStart + 3; j++) {
                if (board[i][j] == num) {
                    return false;
                }
            }
        }

        return true;
    }

    // 生成带提示数的数独题目
    public int[][] generatePuzzle(int hints) {
        int[][] puzzle = generateFullBoard();
        Random rand = new Random();
        int[][] mask = new int[SIZE][SIZE];

        // 确保每行、每列、每个 3x3 宫格的空格尽可能均匀分布
        int spacesToFill = SIZE * SIZE - hints;
        while (spacesToFill > 0) {
            int row = rand.nextInt(SIZE);
            int col = rand.nextInt(SIZE);
            if (mask[row][col] == 0) {
                mask[row][col] = 1; // 标记为被掩盖的位置
                puzzle[row][col] = 0; // 将值设为 0
                spacesToFill--;
            }
        }
        return puzzle;
    }
}

class SudokuSolver {
    private static final int SIZE = 9; // 9x9 数独

    // 检查数独的有效性
    private boolean isValid(int[][] board, int row, int col, int num) {
        for (int i = 0; i < SIZE; i++) {
            if (board[row][i] == num || board[i][col] == num) {
                return false;
            }
        }

        int boxRowStart = row / 3 * 3;
        int boxColStart = col / 3 * 3;
        for (int i = boxRowStart; i < boxRowStart + 3; i++) {
            for (int j = boxColStart; j < boxColStart + 3; j++) {
                if (board[i][j] == num) {
                    return false;
                }
            }
        }

        return true;
    }

    // 使用回溯法解决数独
    public boolean solve(int[][] board) {
        for (int row = 0; row < SIZE; row++) {
            for (int col = 0; col < SIZE; col++) {
                if (board[row][col] == 0) {
                    for (int num = 1; num <= SIZE; num++) {
                        if (isValid(board, row, col, num)) {
                            board[row][col] = num;
                            if (solve(board)) {
                                return true;
                            } else {
                                board[row][col] = 0; // 回溯
                            }
                        }
                    }
                    return false; // 如果没有数字能放入，返回 false
                }
            }
        }
        return true; // 成功解出数独
    }
}

public class SudokuTest {

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("请输入提示数（1~81）：");
        int hints = scanner.nextInt();

        // 生成数独
        SudokuGenerator generator = new SudokuGenerator();
        int[][] puzzle = generator.generatePuzzle(hints);

        System.out.println("生成的数独题目：");
        printBoard(puzzle);

        // 求解数独
        SudokuSolver solver = new SudokuSolver();
        if (solver.solve(puzzle)) {
            System.out.println("求解后的数独：");
            printBoard(puzzle);
        } else {
            System.out.println("无法解出该数独题目");
        }
    }

    // 打印数独板
    private static void printBoard(int[][] board) {
        for (int i = 0; i < board.length; i++) {
            for (int j = 0; j < board[i].length; j++) {
                System.out.print(board[i][j] + " ");
            }
            System.out.println();
        }
    }
}
