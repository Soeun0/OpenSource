use std::io;

fn main() {
    let mut input = String::new();

    // 행렬의 크기 입력
    println!("Enter the number of rows:");
    io::stdin().read_line(&mut input).unwrap();
    let rows: usize = input.trim().parse().unwrap();
    input.clear();

    println!("Enter the number of columns:");
    io::stdin().read_line(&mut input).unwrap();
    let cols: usize = input.trim().parse().unwrap();
    input.clear();

    // 첫 번째 행렬 입력
    println!("Enter elements of the first matrix:");
    let mut matrix1 = vec![vec![0; cols]; rows];
    for i in 0..rows {
        for j in 0..cols {
            io::stdin().read_line(&mut input).unwrap();
            matrix1[i][j] = input.trim().parse().unwrap();
            input.clear();
        }
    }

    // 두 번째 행렬 입력
    println!("Enter elements of the second matrix:");
    let mut matrix2 = vec![vec![0; cols]; rows];
    for i in 0..rows {ㄷ
        for j in 0..cols {
            io::stdin().read_line(&mut input).unwrap();
            matrix2[i][j] = input.trim().parse().unwrap();
            input.clear();
        }
    }

    // 두 행렬의 합 계산
    let mut result = vec![vec![0; cols]; rows];
    for i in 0..rows {
        for j in 0..cols {
            result[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }

    // 결과 출력
    println!("The resulting matrix is:");
    for row in result {
        for val in row {
            print!("{} ", val);
        }
        println!();
    }
}
