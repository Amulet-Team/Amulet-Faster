import unittest
import tempfile

from amulet.faster import FasterKV


class FasterPythonTestCase(unittest.TestCase):
    def test_py(self) -> None:
        with tempfile.TemporaryDirectory() as tempdir:
            faster_kv = FasterKV(tempdir)
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


if __name__ == "__main__":
    unittest.main()
