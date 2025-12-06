program main
  implicit none
  INTEGER,PARAMETER :: ll = SELECTED_INT_KIND(25)
  INTEGER,PARAMETER :: BP = SELECTED_REAL_KIND(15)

  REAL(KIND=BP) :: start, finish

  CHARACTER(LEN=:),ALLOCATABLE :: arr(:)
  CHARACTER(LEN=:),ALLOCATABLE :: fn
  CHARACTER(LEN=5000) :: input
  ! CHARACTER(LEN=15) :: input
  
  INTEGER(KIND=ll) :: part1, part2, number
  INTEGER(KIND=ll) :: IO, ios, sz, col, it, it2, pos, new_pos, tmp, offset


  CALL cpu_time(start)

  fn = "input.txt"

  OPEN(NEWUNIT=IO,FILE=FN,STATUS="OLD",FORM="FORMATTED",ACCESS="SEQUENTIAL")
  col = 0
  sz = 0
  DO
    READ(IO,'(A)',iostat=ios) input
    IF (is_iostat_end(ios)) EXIT
    col = col+1
    sz = MAX(LEN(input),0)
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
        number = number * tmp
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
      IF (tmp.eq.0) EXIT
      IF (arr(col)(pos:pos).eq."*") THEN
        number = number * tmp
      ELSE
        number = number + tmp
      END IF
    END DO
    part2 = part2 + number
    pos = new_pos
  END DO

  CALL cpu_time(finish)

  PRINT *, "Time (s): ", finish-start
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