#include <gtest/gtest.h>

#include "../Model/s21_object.h"

TEST(valid_file, 1) {
  EXPECT_ANY_THROW({ s21::Facade m("WwOdef.obj"); });
}

TEST(valid_file, 2) {
  EXPECT_ANY_THROW({ s21::Facade m("ATest/resources/cube.obj"); });
}

TEST(valid_file, 3) {
  EXPECT_ANY_THROW({ s21::Facade m("cube.ob"); });
}

TEST(valid_file, 4) {
  EXPECT_ANY_THROW({ s21::Facade m("./dath/Test/resources/cube.obj"); });
}

TEST(valid_file, 5) {
  EXPECT_ANY_THROW({ s21::Facade m("Makefile"); });
}

TEST(valid_file, 6) {
  EXPECT_ANY_THROW({ s21::Facade m("/"); });
}

TEST(valid_file, 7) {
  EXPECT_ANY_THROW({ s21::Facade m("Test/resources/cube.objl"); });
}

TEST(move_x, 1) {
  s21::Facade f("Test/resources/cube.obj");
  std::vector<double> m_old = f.GetMatrix3d();

  f.Transformation(s21::type_transf::moving, 2.5, 0, 0);

  std::vector<double> m_new = f.GetMatrix3d();

  EXPECT_TRUE(f.GetCountOfVertexes() == 8);
  EXPECT_TRUE(f.GetCountOfFacets() == 6);

  for (int i = 0; i < m_new.size(); i += 3) {
    EXPECT_TRUE(fabsl(m_new[i] - (m_old[i] + 2.5)) < 1e-7);
  }
}

TEST(move_x, 2) {
  s21::Facade f("Test/resources/cube.obj");
  std::vector<double> m_old = f.GetMatrix3d();

  f.Transformation(s21::type_transf::moving, -3.14, 0, 0);

  std::vector<double> m_new = f.GetMatrix3d();

  for (int i = 0; i < m_new.size(); i += 3) {
    EXPECT_TRUE(fabsl(m_new[i] - (m_old[i] - 3.14)) < 1e-7);
  }
}

TEST(move_y, 1) {
  s21::Facade f("Test/resources/cube.obj");
  std::vector<double> m_old = f.GetMatrix3d();

  f.Transformation(s21::type_transf::moving, 0, 1.99, 0);

  std::vector<double> m_new = f.GetMatrix3d();

  for (int i = 1; i < m_new.size(); i += 3) {
    EXPECT_TRUE(fabsl(m_new[i] - (m_old[i] + 1.99)) < 1e-7);
  }
}

TEST(move_y, 2) {
  s21::Facade f("Test/resources/cube.obj");
  std::vector<double> m_old = f.GetMatrix3d();

  f.Transformation(s21::type_transf::moving, 0, -13.7654, 0);

  std::vector<double> m_new = f.GetMatrix3d();

  for (int i = 1; i < m_new.size(); i += 3) {
    EXPECT_TRUE(fabsl(m_new[i] - (m_old[i] - 13.7654)) < 1e-7);
  }
}

TEST(move_z, 1) {
  s21::Facade f("Test/resources/cube.obj");
  std::vector<double> m_old = f.GetMatrix3d();

  f.Transformation(s21::type_transf::moving, 0, 0, 100);

  std::vector<double> m_new = f.GetMatrix3d();

  for (int i = 2; i < m_new.size(); i += 3) {
    EXPECT_TRUE(fabsl(m_new[i] - (m_old[i] + 100)) < 1e-7);
  }
}

TEST(move_z, 2) {
  s21::Facade f("Test/resources/cube.obj");
  std::vector<double> m_old = f.GetMatrix3d();

  f.Transformation(s21::type_transf::moving, 0, 0, -99.432);

  std::vector<double> m_new = f.GetMatrix3d();

  for (int i = 2; i < m_new.size(); i += 3) {
    EXPECT_TRUE(fabsl(m_new[i] - (m_old[i] - 99.432)) < 1e-7);
  }
}

TEST(move_all, 1) {
  s21::Facade f("Test/resources/cube.obj");
  std::vector<double> m_old = f.GetMatrix3d();

  f.Transformation(s21::type_transf::moving, 1.00003, 456.04, -99.432);

  std::vector<double> m_new = f.GetMatrix3d();

  for (int i = 0; i < m_new.size(); i += 3) {
    EXPECT_TRUE(fabsl(m_new[i] - (m_old[i] + 1.00003)) < 1e-7);
    EXPECT_TRUE(fabsl(m_new[i + 1] - (m_old[i + 1] + 456.04)) < 1e-7);
    EXPECT_TRUE(fabsl(m_new[i + 2] - (m_old[i + 2] - 99.432)) < 1e-7);
  }
}

TEST(dont_move, 1) {
  s21::Facade f("Test/resources/cube.obj");
  std::vector<double> m_old = f.GetMatrix3d();

  f.Transformation(s21::type_transf::moving, 0, 0, 0);

  std::vector<double> m_new = f.GetMatrix3d();

  for (int i = 0; i < m_new.size(); ++i) {
    EXPECT_TRUE(fabsl(m_new[i] - m_old[i]) < 1e-7);
  }
}

TEST(turn_x, 1) {
  s21::Facade f("Test/resources/cube.obj");
  f.Transformation(s21::type_transf::rotation, 30, 0, 0);

  std::vector<double> m_turn = f.GetMatrix3d();
  std::vector<double> m_new{
      -0.5, -0.1830127, -0.6830127, -0.5, 0.6830127,  -0.1830127,
      0.5,  0.6830127,  -0.1830127, 0.5,  -0.1830127, -0.6830127,
      -0.5, -0.6830127, 0.1830127,  -0.5, 0.1830127,  0.6830127,
      0.5,  0.1830127,  0.6830127,  0.5,  -0.6830127, 0.1830127};

  for (int i = 0; i < m_turn.size(); ++i) {
    EXPECT_TRUE(fabsl(m_turn[i] - m_new[i]) < 1e-7);
  }
}

TEST(turn_x, 2) {
  s21::Facade f("Test/resources/cube.obj");
  f.Transformation(s21::type_transf::rotation, -45, 0, 0);

  std::vector<double> m_turn = f.GetMatrix3d();
  std::vector<double> m_new{-0.5, -0.70710678,    -5.5511151e-17,
                            -0.5, 5.5511151e-17,  -0.70710678,
                            0.5,  5.5511151e-17,  -0.70710678,
                            0.5,  -0.70710678,    -5.5511151e-17,
                            -0.5, -5.5511151e-17, 0.70710678,
                            -0.5, 0.70710678,     5.5511151e-17,
                            0.5,  0.70710678,     5.5511151e-17,
                            0.5,  -5.5511151e-17, 0.70710678};

  for (int i = 0; i < m_turn.size(); ++i) {
    EXPECT_TRUE(fabsl(m_turn[i] - m_new[i]) < 1e-7);
  }
}

TEST(turn_y, 1) {
  s21::Facade f("Test/resources/cube.obj");
  f.Transformation(s21::type_transf::rotation, 0, 60, 0);

  std::vector<double> m_turn = f.GetMatrix3d();
  std::vector<double> m_new{
      -0.6830127, -0.5, 0.1830127,  -0.6830127, 0.5,  0.1830127,
      -0.1830127, 0.5,  -0.6830127, -0.1830127, -0.5, -0.6830127,
      0.1830127,  -0.5, 0.6830127,  0.1830127,  0.5,  0.6830127,
      0.6830127,  0.5,  -0.1830127, 0.6830127,  -0.5, -0.1830127};

  for (int i = 0; i < m_turn.size(); ++i) {
    EXPECT_TRUE(fabsl(m_turn[i] - m_new[i]) < 1e-7);
  }
}

TEST(turn_y, 2) {
  s21::Facade f("Test/resources/cube.obj");
  f.Transformation(s21::type_transf::rotation, 0, -45, 0);

  std::vector<double> m_turn = f.GetMatrix3d();
  std::vector<double> m_new{-0.00000000, -0.50000000, -0.70710678, -0.00000000,
                            0.50000000,  -0.70710678, 0.70710678,  0.50000000,
                            -0.00000000, 0.70710678,  -0.50000000, -0.00000000,
                            -0.70710678, -0.50000000, 0.00000000,  -0.70710678,
                            0.50000000,  0.00000000,  0.00000000,  0.50000000,
                            0.70710678,  0.00000000,  -0.50000000, 0.70710678};

  for (int i = 0; i < m_turn.size(); ++i) {
    EXPECT_TRUE(fabsl(m_turn[i] - m_new[i]) < 1e-7);
  }
}

TEST(turn_z, 1) {
  s21::Facade f("Test/resources/cube.obj");
  f.Transformation(s21::type_transf::rotation, 0, 0, 45);

  std::vector<double> m_turn = f.GetMatrix3d();
  std::vector<double> m_new{
      -5.5511151e-17, -0.70710678, -0.5, -0.70710678, 5.5511151e-17,  -0.5,
      5.5511151e-17,  0.70710678,  -0.5, 0.70710678,  -5.5511151e-17, -0.5,
      -5.5511151e-17, -0.70710678, 0.5,  -0.70710678, 5.5511151e-17,  0.5,
      5.5511151e-17,  0.70710678,  0.5,  0.70710678,  -5.5511151e-17, 0.5,
  };

  for (int i = 0; i < m_turn.size(); ++i) {
    EXPECT_TRUE(fabsl(m_turn[i] - m_new[i]) < 1e-7);
  }
}

TEST(turn_z, 2) {
  s21::Facade f("Test/resources/cube.obj");
  f.Transformation(s21::type_transf::rotation, 0, 0, -60);

  std::vector<double> m_turn = f.GetMatrix3d();
  std::vector<double> m_new{
      -0.6830127, 0.1830127,  -0.5, 0.1830127,  0.6830127,  -0.5,
      0.6830127,  -0.1830127, -0.5, -0.1830127, -0.6830127, -0.5,
      -0.6830127, 0.1830127,  0.5,  0.1830127,  0.6830127,  0.5,
      0.6830127,  -0.1830127, 0.5,  -0.1830127, -0.6830127, 0.5};

  for (int i = 0; i < m_turn.size(); ++i) {
    EXPECT_TRUE(fabsl(m_turn[i] - m_new[i]) < 1e-7);
  }
}

TEST(dont_turn, 1) {
  s21::Facade f("Test/resources/cube.obj");
  std::vector<double> m_old = f.GetMatrix3d();

  f.Transformation(s21::type_transf::rotation, 0, 0, 0);

  std::vector<double> m_new = f.GetMatrix3d();

  for (int i = 0; i < m_new.size(); ++i) {
    EXPECT_TRUE(fabsl(m_new[i] - m_old[i]) < 1e-7);
  }
}

TEST(turn_all, 1) {
  s21::Facade f("Test/resources/cube.obj");
  f.Transformation(s21::type_transf::rotation, -11, 78, -99);
  std::vector<double> m_turn = f.GetMatrix3d();
  std::vector<double> m_new{-0.50223458, 0.57638738,  0.40686363,  0.49650397,
                            0.60716858,  0.36719221,  0.46397942,  0.40181663,
                            -0.61095539, -0.53475913, 0.37103543,  -0.57128397,
                            -0.46397942, -0.40181663, 0.61095539,  0.53475913,
                            -0.37103543, 0.57128397,  0.50223458,  -0.57638738,
                            -0.40686363, -0.49650397, -0.60716858, -0.36719221};

  for (int i = 0; i < m_turn.size(); ++i) {
    EXPECT_TRUE(fabsl(m_turn[i] - m_new[i]) < 1e-7);
  }
}

TEST(scale, 1) {
  s21::Facade f("Test/resources/cube.obj");
  std::vector<double> m_old = f.GetMatrix3d();

  f.Transformation(s21::type_transf::scaling, 2.5);

  std::vector<double> m_new = f.GetMatrix3d();

  for (int i = 0; i < m_new.size(); i += 3) {
    EXPECT_TRUE(fabsl(m_new[i] - (m_old[i] * 2.5)) < 1e-7);
  }
}

TEST(scale, 2) {
  s21::Facade f("Test/resources/cube.obj");
  std::vector<double> m_old = f.GetMatrix3d();

  f.Transformation(s21::type_transf::scaling, 10);

  std::vector<double> m_new = f.GetMatrix3d();

  for (int i = 0; i < m_new.size(); i += 3) {
    EXPECT_TRUE(fabsl(m_new[i] - (m_old[i] * 10)) < 1e-7);
  }
}

TEST(scale, 3) {
  s21::Facade f("Test/resources/cube.obj");
  std::vector<double> m_old = f.GetMatrix3d();

  f.Transformation(s21::type_transf::scaling, 0.25);

  std::vector<double> m_new = f.GetMatrix3d();

  for (int i = 0; i < m_new.size(); i += 3) {
    EXPECT_TRUE(fabsl(m_new[i] - (m_old[i] * 0.25)) < 1e-7);
  }
}

TEST(scale, 4) {
  s21::Facade f("Test/resources/cube.obj");
  std::vector<double> m_old = f.GetMatrix3d();

  f.Transformation(s21::type_transf::scaling, 0.0001);

  std::vector<double> m_new = f.GetMatrix3d();

  for (int i = 0; i < m_new.size(); i += 3) {
    EXPECT_TRUE(fabsl(m_new[i] - (m_old[i] * 0.0001)) < 1e-7);
  }
}

TEST(scale_valid, 1) {
  s21::Facade f("Test/resources/cube.obj");
  std::vector<double> m_old = f.GetMatrix3d();

  f.Transformation(s21::type_transf::scaling, 0);

  std::vector<double> m_new = f.GetMatrix3d();

  for (int i = 0; i < m_new.size(); i += 3) {
    EXPECT_TRUE(fabsl(m_new[i] - m_old[i]) < 1e-7);
  }
}

TEST(scale_valid, 2) {
  s21::Facade f("Test/resources/cube.obj");
  std::vector<double> m_old = f.GetMatrix3d();

  f.Transformation(s21::type_transf::scaling, -23);

  std::vector<double> m_new = f.GetMatrix3d();

  for (int i = 0; i < m_new.size(); i += 3) {
    EXPECT_TRUE(fabsl(m_new[i] - m_old[i]) < 1e-7);
  }
}

TEST(transf_all, 1) {
  s21::Facade f("Test/resources/cube.obj");

  f.Transformation(s21::type_transf::moving, 1.00003, 456.04, -99.432);
  f.Transformation(s21::type_transf::rotation, -11, 78, -99);
  f.Transformation(s21::type_transf::scaling, 6.33);

  std::vector<double> transf = f.GetMatrix3d();
  std::vector<double> result{
      2855.62873512, 706.89178708,  -246.59351007, 2861.95075016, 707.08663206,
      -246.84463016, 2861.74486973, 705.78675420,  -253.03630447, 2855.42285470,
      705.59190922,  -252.78518438, 2855.87089027, 700.69975567,  -245.30160918,
      2862.19290531, 700.89460065,  -245.55272927, 2861.98702488, 699.59472279,
      -251.74440359, 2855.66500985, 699.39987781,  -251.49328349,
  };

  for (int i = 0; i < transf.size(); ++i) {
    EXPECT_TRUE(fabsl(transf[i] - result[i]) < 1e-7);
  }
}

TEST(restore, 1) {
  s21::Facade f("Test/resources/cube.obj");

  std::vector<double> result{f.GetMatrix3d()};

  f.Transformation(s21::type_transf::moving, 1.00003, 456.04, -99.432);
  f.Transformation(s21::type_transf::rotation, -11, 78, -99);
  f.Transformation(s21::type_transf::scaling, 6.33);

  f.RestoreOriginal();
  std::vector<double> transf = f.GetMatrix3d();

  EXPECT_TRUE(f.GetCountOfVertexes() == 8);
  EXPECT_TRUE(f.GetCountOfFacets() == 6);

  for (int i = 0; i < result.size(); ++i) {
    EXPECT_TRUE(fabsl(transf[i] - result[i]) < 1e-7);
  }
}

TEST(new_object, 2) {
  s21::Facade f("Test/resources/cube.obj");
  double old_length = f.GetMaxLength();
  std::vector<double> old_m{f.GetMatrix3d()};
  std::vector<int> old_v{f.GetVertexes()};

  f.Set2DTextures("Test/resources/cube.obj");
  std::vector<double> new_m{f.GetMatrix3d()};
  std::vector<int> new_v{f.GetVertexes()};

  EXPECT_TRUE(old_v == new_v);
  EXPECT_TRUE(old_m == new_m);
  EXPECT_TRUE(fabsl(f.GetMaxLength() - old_length) < 1e-7);
}

int main() {
  testing::InitGoogleTest();

  return RUN_ALL_TESTS();
}
