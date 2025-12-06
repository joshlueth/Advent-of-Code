program main
  implicit none
  INTEGER,PARAMETER :: ll = SELECTED_INT_KIND(25)
  INTEGER,PARAMETER :: BP = SELECTED_REAL_KIND(15)

  INTEGER(KIND=BP) :: start, finish
  REAL(KIND=BP) :: start_r, finish_r

  CHARACTER(LEN=:),ALLOCATABLE :: arr(:)
  CHARACTER(LEN=:),ALLOCATABLE :: fn
  CHARACTER(LEN=5000) :: input
  ! CHARACTER(LEN=15) :: input
  
  INTEGER(KIND=ll) :: part1, part2, number
  INTEGER(KIND=ll) :: IO, ios, sz, col, it, it2, pos, new_pos, tmp, offset


  CALL system_clock(start)
  CALL cpu_time(start_r)

  fn = "input.txt"

  OPEN(NEWUNIT=IO,FILE=FN,STATUS="OLD",FORM="FORMATTED",ACCESS="SEQUENTIAL")
  col = 0
  sz = 0
  DO
    READ(IO,'(A)',iostat=ios) input
    IF (is_iostat_end(ios)) EXIT
    col = col+1
    sz = MAX(LEN(TRIM(input)),sz)
  END DO
  REWIND(IO)
  ALLOCATE(CHARACTER(LEN=sz) :: arr(1:col))
  it = 1
  DO
    READ(IO,'(A)',iostat=ios) input
    IF (is_iostat_end(ios)) EXIT
    arr(it) = input
    it = it+1
  END DO
  CLOSE(IO)

  part1 = 0

  pos = 1
  DO WHILE (pos .lt. len(arr(col)))
    new_pos = Find_op(arr(col),pos+1)
    IF (arr(col)(pos:pos).eq."*") THEN
      number = 1
    ELSE
      number = 0
    END IF
    DO it=1,col-1
      READ(arr(it)(pos:new_pos-1),'(i10)') tmp
      IF (arr(col)(pos:pos).eq."*") THEN
        number = number * MAX(tmp,1)
      ELSE
        number = number + tmp
      END IF
    END DO
    part1 = part1 + number
    pos = new_pos
  END DO

  part2 = 0
  pos = 1
  offset = -2
  DO WHILE (pos .lt. len(arr(col)))
    new_pos = Find_op(arr(col),pos+1)
    IF (arr(col)(pos:pos).eq."*") THEN
      number = 1
    ELSE
      number = 0
    END IF
    IF (new_pos.ge.len(arr(col))) offset = -1
    DO it=pos,new_pos+offset
      BLOCK
        CHARACTER(LEN=:),ALLOCATABLE :: tmp_str
        tmp_str = ""
        DO it2=1,col-1
          tmp_str = tmp_str//arr(it2)(it:it)
        END DO
        READ(tmp_str,'(i10)') tmp
      END BLOCK
      IF (arr(col)(pos:pos).eq."*") THEN
        number = number * tmp
      ELSE
        number = number + tmp
      END IF
    END DO
    part2 = part2 + number
    pos = new_pos
  END DO

  CALL cpu_time(finish_r)
  CALL system_clock(finish)
  CALL system_clock(count_rate=tmp)

  PRINT *, "Time (s):   ", finish_r-start_r
  PRINT *, "Time (sys): ", finish-start
  PRINT *, "Time (sys): ", (finish-start)/REAL(tmp,KIND=BP)
  PRINT *, tmp
  PRINT *, "Part 1: ", part1
  PRINT *, "Part 2: ", part2

CONTAINS

FUNCTION Find_op(ch,begin) RESULT(loc)
  CHARACTER(LEN=*),INTENT(IN) :: ch
  INTEGER(kind=ll),INTENT(IN) :: begin
  INTEGER(kind=ll) :: loc, iter

  loc = len(ch)+1 
  DO iter=begin,len(ch)
    IF (ch(iter:iter).ne." ") THEN
      loc = iter
      RETURN
    END IF
  END DO

  RETURN
END FUNCTION Find_op

end program main