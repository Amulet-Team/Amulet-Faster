import unittest
import tempfile

from amulet.faster import FasterKV


class FasterPythonTestCase(unittest.TestCase):
    def test_py(self) -> None:
        with tempfile.TemporaryDirectory() as tempdir:
            faster_kv = FasterKV(r"C:\Users\james\Downloads\db")
            faster_kv.set(1, 2)
            self.assertEqual(2, faster_kv.get(1))
            faster_kv.set(1, 3)
            self.assertEqual(3, faster_kv.get(1))
            faster_kv.set(1, 4)
            faster_kv.set(2, 5)
            self.assertEqual(4, faster_kv.get(1))
            self.assertEqual(5, faster_kv.get(2))
            self.assertEqual(None, faster_kv.get(3))
            faster_kv.remove(2)
            self.assertEqual(None, faster_kv.get(2))

            step = 20_000

            for i in range(100):
                print(f"set and get {i}%")
                for j in range(step):
                    v = i * step + j
                    faster_kv.set(v, v + 1)
                    # self.assertEqual(v + 1, faster_kv.get(v))

            faster_kv.compact()

            for i in range(100):
                print(f"get {i}%")
                for j in range(step):
                    v = i * step + j
                    self.assertEqual(v + 1, faster_kv.get(v))

            for i in range(100):
                print(f"remove {i}%")
                for j in range(step):
                    v = i * step + j
                    faster_kv.remove(v)

            for i in range(100):
                print(f"get None {i}%")
                for j in range(step):
                    v = i * step + j
                    self.assertEqual(None, faster_kv.get(v))


if __name__ == "__main__":
    unittest.main()
