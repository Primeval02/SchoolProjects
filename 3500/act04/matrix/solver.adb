-- ----------------------------------------------
-- NAME: Bradley Tate
-- ASGT: Activity 4
-- ORGN: CSUB - CMPS 3500
-- FILE: solver.adb
-- DATE: 03/21/2023
-- ----------------------------------------------

with Ada.Integer_Text_IO; use Ada.Integer_Text_IO;
with Ada.Directories; use Ada.Directories;
with Ada.Text_IO; use Ada.Text_IO;
with Ada.Strings.Fixed; use Ada.Strings.Fixed;
with Ada.Float_Text_IO; use Ada.Float_Text_IO;


-- Find the inverse of A and Multiply with B to get solution
-- Inverse of A is 1/det(A) * (cofactor matrix)^T

procedure solver is

    N : INTEGER;

    type MATRIX is array(NATURAL range <>, NATURAL range <>) of LONG_FLOAT;

    -- A
    m1: MATRIX (1..6, 1..6);
    -- B
    m2: MATRIX (1..6, 1..1);
    -- C 
    m3: MATRIX (1..6, 1..1);

    Directory : String := ".";
    Pattern : String := "";
    Search : Search_Type;
    Dir_Ent : Directory_Entry_Type;
    smatch : String := "txt";
    Inf : FILE_TYPE;

    -- Matrix Creation and Printing
    procedure PopulateMatrixLF(Inf : in FILE_TYPE; Result : out MATRIX) is
    begin
        for Row in Integer range 1..6 loop
            for Column in Integer range 1..6 loop
                Ada.Integer_Text_IO.Get(inf, N);
                Result(Row, Column) := LONG_FLOAT(N);
            end loop;
        end loop;
    end PopulateMatrixLF;


    procedure PopulateMatrixB(Inf : in FILE_TYPE; Result : out MATRIX) is
    begin
        for Row in Integer range 1..6 loop
            Ada.Integer_Text_IO.Get(inf, N);
            Result(Row, 1) := LONG_FLOAT(N);
        end loop;
    end PopulateMatrixB;

    procedure PrintMatrixLI (Input : in MATRIX) is
    begin
        Set_Output(Standard_Output);
        for Row in Integer range 1..6 loop
            for Column in Integer range 1..6 loop
                Ada.Text_IO.Put(Long_Integer'Image(Long_Integer(Input(Row, Column))));
                Put (" ");
            end loop;
            New_Line;
        end loop;
    end PrintMatrixLI;

    procedure PrintMatrixB (Input : in MATRIX) is
    begin
        Set_Output(Standard_Output);
        for Row in Integer range 1..6 loop
            Ada.Text_IO.Put(Long_Integer'Image(Long_Integer(Input(Row, 1))));
            Put (" ");
            New_Line;
        end loop;
    end PrintMatrixB;



    -- Matrix Manip.
    function Determinant(mdet: in out MATRIX; n: INTEGER) return LONG_FLOAT is
        i : INTEGER;
        aj : INTEGER;
        bj : INTEGER;
        k : LONG_FLOAT;
        d : LONG_FLOAT;
        l : LONG_FLOAT;
        sign : LONG_FLOAT;
        b : MATRIX(1..n-1, 1..n-1);

    begin
        i := 2;
        sign := 1.0;
        l := 0.0;
        k := 1.0;
        if n = 2 then
            d := (mdet(1,1) * mdet(2,2)) - (mdet(1,2) * mdet(2,1));
            return d;
        end if;

        for k in 1..n loop
            aj := 1;
            bj := 1;
            for i in 2..n loop
                bj := 1;
                for aj in 1..n loop
                    if aj = k then
                        goto endofloop;
                    end if;
                    if bj = n then
                        goto endofloop;
                    end if;
                    b(i-1, bj) := mdet(i, aj);
                    bj := bj + 1;
                    <<endofloop>>
                end loop;
            end loop;
            l := l + (sign * mdet(1, k) * Determinant(b, n-1));
            sign := sign * (-1.0);

        end loop;
        return l;
    end Determinant;


    function Transpose(Matrix_1 : in MATRIX) return MATRIX is
        Result : MATRIX (1..6,1..6);
    begin
        for Row in Integer range 1..6 loop
            for Column in Integer range 1..6 loop
                Result(Row, Column) := Matrix_1(Column, Row);
            end loop;
        end loop;
        return Result;
    end Transpose;


    function MultiplyScalar(Matrix_1 : in MATRIX; Scalar : in Long_Float) return MATRIX is
        Product : Long_float;
        Result : MATRIX (1..6,1..6);
    begin
        for Row in Integer range 1..6 loop
            for Column in Integer range 1..6 loop
                Product := Matrix_1(Row, Column) * Scalar;
                Result(Row, Column) := Product;
            end loop;
        end loop;
        return Result;
    end MultiplyScalar;

-- Solver func. ~ "Main"
det : LONG_FLOAT;

begin

    -- Read Text Files and Populate Matrices
    Start_Search (Search, Directory, Pattern);
    while More_Entries (Search) loop
        Get_Next_Entry (Search, Dir_Ent);
        if Tail (Simple_Name (Dir_Ent), smatch'Length) = smatch then
            Open(Inf, In_File, Simple_Name (Dir_Ent));

            if (Simple_Name (Dir_Ent)) = "A.txt" then
                PopulateMatrixLF(Inf, m1);

            elsif (Simple_Name (Dir_Ent)) = "B.txt" then
                PopulateMatrixB(Inf, m2);
            end if;

            Close(Inf);
        end if;
    end loop;
    End_Search (Search);

    det := Determinant(m1, 6);


    -- Final Printing
    Put_Line("a = ");
    Put_Line("b = ");
    Put_Line("c = ");
    Put_Line("d = ");
    Put_Line("e = ");
    Put_Line("f = ");
    New_Line;
    Put_Line("Testing:");
    Put_Line("m1:");
    PrintMatrixLI(m1);
    New_Line;
    Put_Line("m2:");
    PrintMatrixB(m2);
    New_Line;
    Put_Line("m1 determinant:");
    Put_Line(Long_Float'image(det));


end solver;
